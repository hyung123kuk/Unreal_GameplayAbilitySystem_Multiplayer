// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKProjectileSpell.h"

#include "Actor/HKProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemComponent.h"
#include "HKGameplayTags.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Actor.h"
#include "AbilitySystem/HKAbilitySystemLibrary.h"
#include "Player/HKPlayerController.h"

void UHKProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	DeltaTime = 0;
	if (!IsLocalPlayer())
	{
		PlaySpawnProjectileMontage();
	}
	else if(!bMouseReleasedSpawn)
	{
		FindTargetDataUnderMouse();
	}
	else
	{
		if (IsStandAlone())
			return;

		if (ServerProcess() && !IsListenServerCharacter())
		{
			FindTargetDataUnderMouse(); // 클라이언트에서 데이터가 올 떄 까지 기다립니다.
		}
	}
}

void UHKProjectileSpell::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!IsLocalPlayer())
		return;

	APlayerController* PC = GetCurrentActorInfo()->PlayerController.Get();
	AHKPlayerController* PlayerController = Cast<AHKPlayerController>(PC);

	if (PlayerController->GetClickMouseHitResult().GetActor() == nullptr)
		return;

	DeltaTime += GetWorld()->GetDeltaSeconds();
 	FRotator Rotation = (PlayerController->GetClickMouseHitResult().GetActor()->GetActorLocation() - CurrentActorInfo->AvatarActor->GetActorLocation()).Rotation();
	FRotator SmoothRotator = FMath::RInterpTo(CurrentActorInfo->AvatarActor->GetActorRotation(), Rotation, DeltaTime/ MouseChargingTime, 1.0f);
	CurrentActorInfo->AvatarActor->SetActorRotation(SmoothRotator);
}

void UHKProjectileSpell::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (!IsLocalPlayer())
		return;

	if (bMouseReleasedSpawn)
	{
		if (DeltaTime < MouseChargingTime)
		{
			OnFailedAbility();
			return;
		}

		FindTargetDataUnderMouse();
	}
}


void UHKProjectileSpell::ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData, const FGameplayTag& ActivationTag)
{
	Super::ActivateAbility_TargetDataUnderMouse(TargetData, StartupInputTag);
	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, 0);
	if (!HitResult.bBlockingHit)
	{
		OnFailedAbility();
		return;
	}

	AActor* HitActor = HitResult.GetActor();
	if (HitActor == nullptr || !HitActor->Implements<UCombatInterface>())
	{
		TArray<AActor*> TargetActors = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(TargetData, 1);
		if (TargetActors.Num() > 0)
		{
			HitActor = TargetActors[0];
		}
	}

	if (HitActor == nullptr || !HitActor->Implements<UCombatInterface>())
	{
		OnFailedAbility();
		return;
	}

	if (Cast<ICombatInterface>(HitActor)->IsDead())
	{
		OnFailedAbility();
		return;
	}

	if (IsSameTeam(GetAvatarActorFromActorInfo(), HitActor))
	{
		OnFailedAbility();
		return;
	}

	ActorCombatInterface->SetCombatTarget(HitActor);

	PlaySpawnProjectileMontage();

}

void UHKProjectileSpell::PlaySpawnProjectileMontage()
{
	FacingTarget();
	PlayRandomAttackMontage(FHKGameplayTags::Get().AttackType_Projectile);
}

void UHKProjectileSpell::OccurMontageEvent(const AActor* AvatarActor, const FVector& CombatSocketLocation)
{
	SpawnProjectile(Target->GetActorLocation());
}

void UHKProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	if (!ServerProcess())
	{
		CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
		return;
	}

	if (IsListenServerCharacter())
	{
		CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
	}

	UE_LOG(LogTemp, Log, TEXT("SpawnProjectile"));
	const FVector SocketLocation = ActorCombatInterface->GetCombatSocketLocation(TaggedMontage.SocketTag, TaggedMontage.SocketName);

	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	AHKProjectile* Projectile = MakeProjectile(ProjectileClass, SocketLocation, Rotation);
	const FGameplayEffectSpecHandle SpecHandle = MakeProjctileEffectSpecHandle(Projectile, ProjectileTargetLocation, Damage);
	Projectile->DamageEffectSpecHandle = SpecHandle;
	Projectile->FinishSpawning(Projectile->GetActorTransform());

}
