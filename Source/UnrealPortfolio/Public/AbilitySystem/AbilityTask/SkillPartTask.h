// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/Abilities/HKSkillAbilitiy.h"
#include "SkillPartTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndSkillPartDelegate);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API USkillPartTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	void InitSkillPartTask(UGameplayAbility* OwningAbility, FSkillPartTaskInfo& TaskInfo);
	virtual void Activate() override;

	virtual void OnOccurTriggerTag(FGameplayEventData Payload);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnOccurTriggerTag(FGameplayEventData Payload);

	UPROPERTY()
	FEndSkillPartDelegate EndSkillPartTask;
	
private:
	
	UGameplayAbility* Ability;

	UPROPERTY()
	bool RunImmediatelyNextTask; // ���� Task �ٷ� ����

	UPROPERTY()
	FGameplayTag TriggerTag;

	UPROPERTY()
	FGameplayTag SocketTag;

	UPROPERTY()
	FName SocketName;	
	
};
