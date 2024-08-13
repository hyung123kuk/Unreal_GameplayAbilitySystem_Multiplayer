// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HKGameplayAbility.h"
#include "HKActAbility.generated.h"


/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKActAbility : public UHKGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnOccurMontageEvent(FGameplayEventData Payload) override;	

	UFUNCTION(BlueprintImplementableEvent)
	FGameplayTag K2_GetActTagType();
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnOccurMontageEvent(FGameplayEventData Payload);

};
