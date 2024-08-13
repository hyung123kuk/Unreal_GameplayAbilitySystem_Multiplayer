// Copyright Druid Mechanics


#include "AbilitySystem/AbilityTask/SkillPartTask.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

void USkillPartTask::InitSkillPartTask(UGameplayAbility* OwningAbility, FSkillPartTaskInfo& TaskInfo)
{
	InitTask(*OwningAbility, OwningAbility->GetGameplayTaskDefaultPriority());
	Ability = OwningAbility;
	RunImmediatelyNextTask = TaskInfo.RunImmediatelyNextTask;
	TriggerTag = TaskInfo.TriggerTag;
	SocketTag = TaskInfo.SocketTag;
	SocketName = TaskInfo.SocketName;
}

void USkillPartTask::Activate()
{
	UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(Ability, TriggerTag);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &USkillPartTask::OnOccurTriggerTag);
	WaitGameplayEventTask->ReadyForActivation();
}

void USkillPartTask::OnOccurTriggerTag(FGameplayEventData Payload)
{
	K2_OnOccurTriggerTag(Payload);
	EndSkillPartTask.Broadcast();
}

