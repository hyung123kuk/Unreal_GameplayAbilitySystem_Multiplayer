// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKMeleeAttack.h"
#include "Interaction/CombatInterface.h"
#include "HKGameplayTags.h"
#include "AbilitySystem/HKAbilitySystemLibrary.h"


UHKMeleeAttack::UHKMeleeAttack()
{
	AbilityTags.AddTag(FHKGameplayTags::Get().Abilities_Attack);
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UHKMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	bool HasHitTarget = false;

	FacingTarget();
	PlayRandomAttackMontage(FHKGameplayTags::Get().AttackType_Melee);
}

void UHKMeleeAttack::OccurMontageEvent(const AActor* AvatarActor, const FVector& CombatSocketLocation)
{
	
}


