// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HKCombatAbility.h"
#include "HKMeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKMeleeAttack : public UHKCombatAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData, const FGameplayTag& ActivationTag);
	virtual void MeleeAttack();

	virtual void OccurMontageEvent(const AActor* TargetActor, const FVector& CombatSocketLocation) override;
	
private:

	UPROPERTY(EditDefaultsOnly, Category = "MeleeAttack")
	float Damage;

};
