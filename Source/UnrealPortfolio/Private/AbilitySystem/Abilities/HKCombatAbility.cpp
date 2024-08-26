// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKCombatAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HKGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/HKAbilitySystemLibrary.h"
#include "Player/HKPlayerController.h"
#include "Interaction/MouseTargetActorInterface.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "Character/HKCharacter.h"
#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"
#include "Components/CapsuleComponent.h"
#include "Actor/HKProjectile.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "Interaction/CombatInterface.h"

void UHKCombatAbility::CauseDamage(AActor* TargetActor, float Damage) const
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, FHKGameplayTags::Get().Damage, Damage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

bool UHKCombatAbility::GetLocalPlayerCondition(UHKAbilitySystemComponent* AbilitySystemComponent)
{
	if (bCloserMouseTarget)
	{
		AHKCharacter* PlayerCharacter = Cast<AHKCharacter>(AbilitySystemComponent->GetAvatarActor());
		AHKPlayerController* PlayerController = Cast<AHKPlayerController>(PlayerCharacter->GetLocalViewingPlayerController());
		FHitResult ClickMouseTarget = PlayerController->GetClickMouseHitResult();

		if (ClickMouseTarget.bBlockingHit && (ClickMouseTarget.GetActor() == nullptr || !ClickMouseTarget.GetActor()->Implements<UCombatInterface>()))
		{
			if (bLockOn)
			{
				ClickMouseTarget = PlayerController->GetLockOnHitResult();
			}
		}

		if (!ClickMouseTarget.bBlockingHit)
			return false;

		if (ClickMouseTarget.GetActor() == nullptr)
			return false;

		if (!ClickMouseTarget.GetActor()->Implements<UCombatInterface>())
			return false;

		if (PlayerCharacter->GetDistanceTo(ClickMouseTarget.GetActor()) <= CombatRange)
		{
			return true;
		}

		if (bLockOn)
		{
			PlayerController->LockOnTarget(ClickMouseTarget, CombatRange - LockOnCloserRange, StartupInputTag);
		}
		return false;
	}

	return true;
}

void UHKCombatAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ActorCombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	Team = ActorCombatInterface->GetTeam();
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHKCombatAbility::FindTargetDataUnderMouse()
{
	if (IsLocalPlayer())
	{
		TArray<AActor*> TargetArray;
		AHKCharacter* PlayerCharacter = Cast<AHKCharacter>(GetAvatarActorFromActorInfo());
		AHKPlayerController* PlayerController = Cast<AHKPlayerController>(PlayerCharacter->GetController());
		FHitResult ClickMouseTarget = PlayerController->GetClickMouseHitResult();
		TargetArray.Add(ClickMouseTarget.GetActor());

		UTargetDataUnderMouse* TargetDataUnderMouseTask = UTargetDataUnderMouse::CreateTargetDataUnderMouse(this, TargetArray);
		TargetDataUnderMouseTask->SetMouseTarget(ClickMouseTarget);
		TargetDataUnderMouseTask->SetCurrentPredictionKey(0);
		TargetDataUnderMouseTask->ValidData.AddDynamic(this, &UHKCombatAbility::ActivateAbility_TargetDataUnderMouse);
		TargetDataUnderMouseTask->ReadyForActivation();
	}
}

bool UHKCombatAbility::IsSameTeam(AActor* Actor,AActor* Actor2) const
{
	return UHKAbilitySystemLibrary::IsSameTeam(Actor,Actor2);
}

bool UHKCombatAbility::PlayRandomAttackMontage(const FGameplayTag& AttackType)
{
	TArray<FTaggedMontage> TypeMontages;
	for (FTaggedMontage& Montage : ActorCombatInterface->GetAttackMontages())
	{
		if (Montage.Type == AttackType)
			TypeMontages.Add(Montage);
	}

	TaggedMontage = GetRandomTaggedMontageFromArray(TypeMontages);
	if (TaggedMontage.Montage == nullptr)
	{
		OnFailedAbility();
		return false;
	}

	PlayMontage(TaggedMontage.Montage, TaggedMontage.MontageTag);
	return true;
}

void UHKCombatAbility::FacingPosition(const FVector& TargetPosition) const
{
	ActorCombatInterface->Execute_UpdateFacingTarget(GetAvatarActorFromActorInfo(), TargetPosition);
}

void UHKCombatAbility::FacingTarget()
{
	Target = ActorCombatInterface->GetCombatTarget();
	if (Target != nullptr)
	{
		AHKCharacterBase* TargetCharacter = Cast<AHKCharacterBase>(Target);
		float CorrectionDistance = TargetCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() / 2.f;
		FVector CorrectionVector = (Target->GetActorLocation() - CharacterBase->GetActorLocation()).GetSafeNormal() * CorrectionDistance;

		ActorCombatInterface->Execute_UpdateFacingTarget(GetAvatarActorFromActorInfo(), Target->GetActorLocation() - CorrectionVector);
	}
}

TArray<AActor*> UHKCombatAbility::FindTargetsWithAngle(const FVector& Origin, float Radius, const FVector& Direction, double Angle) const
{
	TArray<AActor*> RadiusActors = FindTargetsWithRadius(Origin, Radius);
	TArray<AActor*> TargetActors;
	for (AActor* RadiusActor : RadiusActors)
	{
		FVector TargetVector = RadiusActor->GetActorLocation() - Origin;
		float angle = cos(FMath::DegreesToRadians(Angle));
		float DotProduct = FVector::DotProduct(TargetVector.GetSafeNormal(), Direction.GetSafeNormal());
		if (angle < DotProduct)
		{
			TargetActors.Add(RadiusActor);
		}
	}

	return TargetActors;
}

TArray<AActor*> UHKCombatAbility::FindTargetsWithRadius(const FVector& Origin, float Radius) const
{
	TArray<FOverlapResult> Overlaps;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AABTA_SphereMultiTrace), false, GetAvatarActorFromActorInfo());
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Projectile, FCollisionShape::MakeSphere(Radius), Params);

	TArray<AActor*> HitActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		if (HitActor && HitActor->Implements<UCombatInterface>() && !IsSameTeam(HitActor, GetAvatarActorFromActorInfo()))
		{
			HitActors.Add(HitActor);
		}
	}

	return HitActors;
}

AHKProjectile* UHKCombatAbility::MakeProjectile(TSubclassOf<AHKProjectile> ProjectileClass, const FVector& ProjectileLocation, const FRotator& ProjectileRotation) const
{
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(ProjectileLocation);
	SpawnTransform.SetRotation(ProjectileRotation.Quaternion());

	AHKProjectile* Projectile = GetWorld()->SpawnActorDeferred<AHKProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	return Projectile;
}

FVector UHKCombatAbility::GetSocketLocation(const FGameplayTag& SocketTag, const FName& SocketName) const
{
	return ActorCombatInterface->GetCombatSocketLocation(SocketTag, SocketName);
}

FGameplayEffectSpecHandle UHKCombatAbility::MakeProjctileEffectSpecHandle(AHKProjectile* Projectile, const FVector& ProjectileTargetLocation, int Damage) const
{
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());

	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);

	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, 1, EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FHKGameplayTags::Get().Damage, Damage);
	Projectile->DamageEffectSpecHandle = SpecHandle;

	return SpecHandle;
}

void UHKCombatAbility::OccurMontageEvent(const AActor* TargetActor, const FVector& CombatSocketLocation)
{
}

void UHKCombatAbility::OnOccurMontageEvent(const FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Log, TEXT("UHKCombatAbility[OnOccurMontageEvent] MontageTagEventTrigger! : %s "), *Payload.EventTag.ToString());
	const FVector CombatSocketLocation = ActorCombatInterface->GetCombatSocketLocation(TaggedMontage.SocketTag, TaggedMontage.SocketName);
	OccurMontageEvent(Target, CombatSocketLocation);
}



