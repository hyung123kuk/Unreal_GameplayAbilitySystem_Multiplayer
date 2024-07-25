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
#include "Item/Inventory.h"
#include "Lobby/HKUILobbyPlayerController.h"
#include "Game/HKGameInstance.h"
#include "Player/HKPlayerState.h"

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

void AHKPlayerController::OnRep_PlayerState()
{
	UHKGameInstance* GameInstance = Cast<UHKGameInstance>(GetGameInstance());
	FString Id = GetPlayerState<AHKPlayerState>()->GetPlayerName();
	FInGamePlayerInfo PlayerInfo = GameInstance->GetPlayerInfoWithID(Id);
	SettingUserInformation(PlayerInfo);
}

void AHKPlayerController::SettingUserInformation(FInGamePlayerInfo PlayerInfo)
{
	if (Inventory == nullptr)
	{
		Inventory = NewObject<UInventory>(this, InventoryClass);
	}
	
	Inventory->ReSettingItems(PlayerInfo.UserItemInfo.ItemIds, PlayerInfo.UserItemInfo.ItemCounts);
	for (FUserItem StartItem : StartItems)
	{
		Inventory->AddItem(StartItem);
	}
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
	HKInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AHKPlayerController::ShiftPressed);
	HKInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AHKPlayerController::ShiftReleased);
	HKInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
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

void AHKPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AHKPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
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
		bTargeting = ThisActor ? true : false;
		ClickMouseTarget = ThisActor ? ThisActor->GetTarget() : nullptr;
		bAutoRunning = false;
	}
}

void AHKPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FHKGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);

	ClickMouseTarget = nullptr;

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

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection);
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

