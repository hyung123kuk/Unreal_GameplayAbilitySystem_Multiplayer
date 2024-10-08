// Copyright Druid Mechanics


#include "Player/HKPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/MouseTargetActorInterface.h"
#include "Input/HKInputComponent.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SplineComponent.h"
#include "HKGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "Lobby/HKUILobbyPlayerController.h"
#include "Game/HKGameInstance.h"
#include "Player/HKPlayerState.h"
#include "UI/Widget/HKSlotWidget.h"
#include "UI/HUD/HKHUD.h"
#include "Character/HKCharacterBase.h"
#include "BlueprintGameplayTagLibrary.h"
#include "Character/HKCharacter.h"
#include "UI/Widget/DamageTextComponent.h"


AHKPlayerController::AHKPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AHKPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
}

void AHKPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	SettingUserSkillInventory();
	InitASCToWidget();
}

void AHKPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	UHKGameInstance* GameInstance = Cast<UHKGameInstance>(GetGameInstance());
	FString Id = GetPlayerState<AHKPlayerState>()->GetPlayerName();
	FInGamePlayerInfo PlayerInfo = GameInstance->GetPlayerInfoWithID(Id);
	SettingUserInventory(PlayerInfo);
	InitASCToWidget();
}

void AHKPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	UHKGameInstance* GameInstance = Cast<UHKGameInstance>(GetGameInstance());
	FString Id = GetPlayerState<AHKPlayerState>()->GetPlayerName();
	FInGamePlayerInfo PlayerInfo = GameInstance->GetPlayerInfoWithID(Id);
	SettingUserInventory(PlayerInfo);
	SettingUserSkillInventory();
	InitASCToWidget();
}

void AHKPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bMiss, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bMiss, bCriticalHit);
	}
}


void AHKPlayerController::SettingUserInventory(FInGamePlayerInfo PlayerInfo)
{
	if (Inventory == nullptr)
	{
		Inventory = NewObject<UInventory>(this, InventoryClass);
		InitHUD();
		InventoryWidget = CreateWidget<UHKSlotWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget->AddToViewport();
		ToggleInventory();
		Inventory->ReSettingItems(PlayerInfo.UserItemInfo.ItemIds, PlayerInfo.UserItemInfo.ItemCounts);

		for (FUserItem StartItem : StartItems)
		{
			Inventory->AddItem(StartItem);
		}

		Inventory->UseItemValueDelegate.AddDynamic(this, &AHKPlayerController::TryUseItem);
	}
}

void AHKPlayerController::SettingUserSkillInventory()
{
	if (SkillWindowWidget == nullptr)
	{
		AHKCharacterBase* CharacterBase = Cast<AHKCharacterBase>(GetCharacter());
		CharacterBase->MakeSkillInventory();
		SkillWindowWidget = CreateWidget<UHKSlotWidget>(GetWorld(), SkillWindowWidgetClass);
		SkillWindowWidget->AddToViewport();
		ToggleSkillWindow();
		CharacterBase->InitSkillInventory();
	}
}

void AHKPlayerController::InitHUD()
{
	AHKPlayerState* HKPlayerState = GetPlayerState<AHKPlayerState>();
	if (HKPlayerState == nullptr)
		return;

	if (AHKHUD* PlayerHUD = Cast<AHKHUD>(GetHUD()))
	{
		PlayerHUD->InitOverlay(this, HKPlayerState, HKPlayerState->GetAbilitySystemComponent(), HKPlayerState->GetAttributeSet());
	}
}

void AHKPlayerController::InitASCToWidget()
{
	if (Inventory == nullptr)
		return;

	if (SkillWindowWidget == nullptr)
		return;

	if (GetASC() == nullptr)
		return;

	if (bInitASCToWidget)
		return;

	bInitASCToWidget = true;
	InventoryWidget->SetAbilitySystemComponent(GetASC());
	SkillWindowWidget->SetAbilitySystemComponent(GetASC());
}


void AHKPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(HKContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(HKContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::None;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AHKPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UHKInputComponent* HKInputComponent = CastChecked<UHKInputComponent>(InputComponent);

	HKInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AHKPlayerController::Move);
	HKInputComponent->BindAction(MouseRightAction, ETriggerEvent::Started, this, &AHKPlayerController::MouseRightPressed);
	HKInputComponent->BindAction(MouseRightAction, ETriggerEvent::Completed, this, &AHKPlayerController::MouseRightReleased);
	HKInputComponent->BindAction(CameraRotateAction, ETriggerEvent::Triggered, this, &AHKPlayerController::CameraRotate);
	HKInputComponent->BindAction(MouseWheelAction, ETriggerEvent::Triggered, this, &AHKPlayerController::CameraZoomInZoomOut);
	HKInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AHKPlayerController::ShiftPressed);
	HKInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AHKPlayerController::ShiftReleased);
	HKInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AHKPlayerController::TryUseItem(FUserItem Item)
{
	TempUseItem = Item;

	FGameplayTagContainer TagContainer = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(FHKGameplayTags::Get().Trigger_UseItem);
	GetASC()->TryActivateAbilitiesByTag(TagContainer);
}

USkillInventory* AHKPlayerController::GetSkillInventory()
{
	return Cast<AHKCharacterBase>(GetCharacter())->GetSkillInventory();;
}

void AHKPlayerController::ToggleInventory()
{
	if (InventoryWidget->GetRenderTransform().Scale.X < 0.1f)
	{
		InventoryWidget->SetRenderScale(FVector2D(1.f, 1.f));
	}
	else
	{
		InventoryWidget->SetRenderScale(FVector2D(0.f,0.f));
	}
}

void AHKPlayerController::ToggleSkillWindow()
{
	if (SkillWindowWidget->GetRenderTransform().Scale.X < 0.1f)
	{
		SkillWindowWidget->SetRenderScale(FVector2D(1.f, 1.f));
	}
	else
	{
		SkillWindowWidget->SetRenderScale(FVector2D(0.f, 0.f));
	}
}

void AHKPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControllerdPawn = GetPawn<APawn>())
	{
		ControllerdPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControllerdPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AHKPlayerController::CameraRotate(const FInputActionValue& InputActionValue)
{
	if (!bMouseRightDown)
		return;

	FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	GetPawn()->AddControllerYawInput(LookAxisVector.X);
	GetPawn()->AddControllerPitchInput(LookAxisVector.Y);
}

void AHKPlayerController::CameraZoomInZoomOut(const FInputActionValue& InputActionValue)
{
	FVector2D Vector = InputActionValue.Get<FVector2D>();

	Cast<AHKCharacter>(GetCharacter())->CameraZoomInZoomOut(Vector.X);
}

void AHKPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (bLockOn)
		{
			if (DistanceToDestination <= LockOnDistanceToOccurTag)
			{
				ClickMouseHitResult = LockOnTargetHitResult;
				ThisActor = Cast<IMouseTargetActorInterface>(LockOnTargetHitResult.GetActor());
				Cast<IMouseTargetActorInterface>(ThisActor)->UnHighlightActor();

				AbilityInputTagHeld(LockOnEventTag);
				bAutoRunning = false;
				bLockOn = false;
				LockOnTargetHitResult = FHitResult();
				LockOnEventTag = FGameplayTag();
				LockOnDistanceToOccurTag = 0.f;
			}
		}
		else if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AHKPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (bLockOn)
	{
		if (LockOnTargetHitResult.bBlockingHit && LockOnTargetHitResult.GetActor() != nullptr)
		{
			IMouseTargetActorInterface* MouseTargetActorInterface = Cast<IMouseTargetActorInterface>(LockOnTargetHitResult.GetActor());
			if(MouseTargetActorInterface != nullptr)
				MouseTargetActorInterface->HighlightActor();
		}
		return;
	}

	if (!CursorHit.bBlockingHit)
		return;

	LastActor = ThisActor;
	ThisActor = Cast<IMouseTargetActorInterface>(CursorHit.GetActor());

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighlightActor();
		if (ThisActor)
		{
			ThisActor->HighlightActor();
		}
	}
}

void AHKPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FHKGameplayTags::Get().InputTag_LMB))
	{
		if (bLockOn)
		{
			bLockOn = false;
			CursorTrace();
		}
		bTargeting = ThisActor ? true : false;
		ClickMouseHitResult = ThisActor ? CursorHit : FHitResult();
		bAutoRunning = false;
	}

	if (InputTag.MatchesTagExact(FHKGameplayTags::Get().InputTag_I))
	{
		ToggleInventory();
	}

	if (InputTag.MatchesTagExact(FHKGameplayTags::Get().InputTag_K))
	{
		ToggleSkillWindow();
	}

	if (InputTag.MatchesTag(FHKGameplayTags::Get().InputTag_Quick))
	{
		QuickSlotPressedDelegate.Broadcast(InputTag);
	}
}

void AHKPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (InputTag.MatchesTag(FHKGameplayTags::Get().InputTag_Quick))
	{
		QuickSlotReleasedDelegate.Broadcast(InputTag);
		return;
	}

	if (!InputTag.MatchesTagExact(FHKGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	ClickMouseHitResult = FHitResult();

	if (!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
					//DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
				}
				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void AHKPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (InputTag.MatchesTag(FHKGameplayTags::Get().InputTag_Quick))
	{
		QuickSlotHeldDelegate.Broadcast(InputTag);
		return;
	}

	if (!InputTag.MatchesTagExact(FHKGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}

	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC() && GetASC()->AbilityInputTagHeld(InputTag))
		{
			return;
		}

		if(ThisActor != nullptr)
			CachedDestination = ThisActor->GetTarget()->GetActorLocation();
	}
	else
	{
		if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;
	}

	FollowTime += GetWorld()->GetDeltaSeconds();

	MoveToDestination(CachedDestination);
}

void AHKPlayerController::MoveToDestination(FVector Destination)
{
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector WorldDirection = (Destination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection);
	}
}

void AHKPlayerController::LockOnTarget(FHitResult TargetResult, float DistanceToOccurTag, FGameplayTag EventTag)
{
	if (!TargetResult.bBlockingHit)
		return;

	if (TargetResult.GetActor() == nullptr)
		return;

	bLockOn = true;
	LockOnTargetHitResult = TargetResult;
	LockOnDistanceToOccurTag = DistanceToOccurTag;
	LockOnEventTag = EventTag;

	CachedDestination = TargetResult.GetActor()->GetActorLocation();
	const APawn* ControlledPawn = GetPawn();
	if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
	{
		Spline->ClearSplinePoints();
		for (const FVector& PointLoc : NavPath->PathPoints)
		{
			Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
			//DrawDebugSphere(GetWorld(), PointLoc, 8.f, 8, FColor::Green, false, 5.f);
		}
		if (NavPath->PathPoints.Num() > 0)
		{
			CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
			bAutoRunning = true;
		}
	}

}


UHKAbilitySystemComponent* AHKPlayerController::GetASC()
{
	if (HKAbilitySystemComponent == nullptr)
	{
		HKAbilitySystemComponent = Cast<UHKAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}

	return HKAbilitySystemComponent;
}

