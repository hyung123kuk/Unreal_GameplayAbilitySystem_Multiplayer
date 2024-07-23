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

void UHKProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

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
		const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
		if (bIsServer)
		{
			FindTargetDataUnderMouse();
		}
	}
}

void UHKProjectileSpell::InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	APlayerController* PC = GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	FRotator Rotation = (CursorHit.Location - CurrentActorInfo->AvatarActor->GetActorLocation()).Rotation();
	CurrentActorInfo->AvatarActor->SetActorRotation(Rotation);
}

void UHKProjectileSpell::InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (bMouseReleasedSpawn)
	{
		FindTargetDataUnderMouse();
	}
}


void UHKProjectileSpell::ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData)
{
	Super::ActivateAbility_TargetDataUnderMouse(TargetData);
	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, 0);
	if (!HitResult.bBlockingHit)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	AActor* HitActor = HitResult.GetActor();
	if (HitActor == nullptr || !HitActor->Implements<UCombatInterface>())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	if (IsSameTeam(GetAvatarActorFromActorInfo(), HitActor))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
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
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;

	UE_LOG(LogTemp, Log, TEXT("SpawnProjectile"));
	const FVector SocketLocation = ActorCombatInterface->GetCombatSocketLocation(TaggedMontage.SocketTag, TaggedMontage.SocketName);

	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());

	AHKProjectile* Projectile = GetWorld()->SpawnActorDeferred<AHKProjectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

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

	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass,1 , EffectContextHandle);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FHKGameplayTags::Get().Damage, Damage);

	Projectile->DamageEffectSpecHandle = SpecHandle;

	Projectile->FinishSpawning(SpawnTransform);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
