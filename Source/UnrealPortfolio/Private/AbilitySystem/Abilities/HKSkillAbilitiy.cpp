// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKSkillAbilitiy.h"
#include "HKGameplayTags.h"
#include "AbilitySystem/AbilityTask/SkillPartTask.h"

void UHKSkillAbilitiy::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	PartCount = 0;
	if (!IsLocalPlayer()) 
	{
		PlayMontage(Montage, EndTag, EndAbilityWhenCompleteMontage);
	}
	else
	{
		FindTargetDataUnderMouse();
	}
}

void UHKSkillAbilitiy::ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData)
{
	Super::ActivateAbility_TargetDataUnderMouse(TargetData);
	PlayMontage(Montage, EndTag, EndAbilityWhenCompleteMontage);
}

void UHKSkillAbilitiy::OccurMontageEvent(const AActor* TargetActor, const FVector& CombatSocketLocation)
{
	OnCompleteAbility();
}

void UHKSkillAbilitiy::CastSkill()
{
	if (PartCount < SkillPartInfo.Num())
	{
		FSkillPartTaskInfo TaskInfo = SkillPartInfo[PartCount];
		USkillPartTask* NewTask = NewObject<USkillPartTask>(TaskInfo.Task);
		NewTask->InitSkillPartTask(this, TaskInfo);
		UAbilityTask::DebugRecordAbilityTaskCreatedByAbility(NewTask);
		PartCount++;
		if (!TaskInfo.RunImmediatelyNextTask)
		{
			NewTask->EndSkillPartTask.AddDynamic(this, &UHKSkillAbilitiy::CastSkill);
		}
		NewTask->ReadyForActivation();

		if (TaskInfo.RunImmediatelyNextTask)
		{
			CastSkill();
		}
	}
	else
	{
		if (EndAbilityWhenCompleteTask)
			OnCompleteAbility();
	}
}
