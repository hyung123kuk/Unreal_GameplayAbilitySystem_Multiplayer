// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HKGameplayAbility.h"
#include "HKTriggerAbility.generated.h"

/**
 * Ŭ���̾�Ʈ���� ������ ���� ����� ��, ����ȭ�� ���� Ability
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
