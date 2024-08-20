// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKMeleeAttack.h"
#include "Interaction/CombatInterface.h"
#include "HKGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/HitResult.h"
#include "AbilitySystem/HKAbilitySystemLibrary.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"

void UHKMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsLocalPlayer())
	{
		MeleeAttack();
	}
	else
	{
		FindTargetDataUnderMouse();
	}
}

void UHKMeleeAttack::ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData)
{
	Super::ActivateAbility_TargetDataUnderMouse(TargetData);
	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData,0);
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

	MeleeAttack();
}

void UHKMeleeAttack::MeleeAttack()
{
	FacingTarget();
	PlayRandomAttackMontage(FHKGameplayTags::Get().AttackType_Melee);
}

void UHKMeleeAttack::OccurMontageEvent(const AActor* TargetActor, const FVector& CombatSocketLocation)
{
	TArray<AActor*> OutOverlappingActors;

	UHKAbilitySystemLibrary::GetLiveOtherTeamActorsWithinRadius(this, Team, OutOverlappingActors, CombatRange, CombatSocketLocation);

	FGameplayCueParameters CueParam;
	CueParam.Location = CombatSocketLocation;
	CueParam.EffectCauser = GetAvatarActorFromActorInfo();
	CueParam.SourceObject = ActorCombatInterface->GetCombatTarget();
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(FHKGameplayTags::Get().GameplayCue_MeleeImpact, CueParam);
	for (auto Enemy : OutOverlappingActors)
	{
		CauseDamage(Enemy, Damage);
	}
	
	if (OutOverlappingActors.Num() > 0)
	{
		DrawDebugSphere(GetWorld(), CombatSocketLocation, CombatRange, 16, FColor::Green,false , 1.5f);
	}
	else
	{
		DrawDebugSphere(GetWorld(), CombatSocketLocation, CombatRange, 16, FColor::Red, false, 1.5f);
	}

	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
}



