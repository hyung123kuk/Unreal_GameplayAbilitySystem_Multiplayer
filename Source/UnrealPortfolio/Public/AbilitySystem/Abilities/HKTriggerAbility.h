// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HKGameplayAbility.h"
#include "HKTriggerAbility.generated.h"

/**
 * 클라이언트에서 아이템 등을 사용할 때, 동기화를 위한 Ability
 */
UCLASS()
class UNREALPORTFOLIO_API UHKTriggerAbility : public UHKGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_SetDataInformation(int& ID, int& UniqueID);

	UFUNCTION(BlueprintCallable)
	void FindTriggerData(int ID, int UniqueID);

	UFUNCTION()
	void ActivateAbility_TargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle);
	
	UFUNCTION(BlueprintImplementableEvent)
	void K2_ActivateAbility_TargetData(int ID, int UniqueID);
};
