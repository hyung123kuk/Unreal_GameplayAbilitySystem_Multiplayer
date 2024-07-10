// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HKAttackAbility.h"
#include "HKMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKMeleeAttack : public UHKAttackAbility
{
	GENERATED_BODY()
	
public:
	UHKMeleeAttack();
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OccurMontageEvent(const AActor* AvatarActor, const FVector& CombatSocketLocation) override;
	
};
