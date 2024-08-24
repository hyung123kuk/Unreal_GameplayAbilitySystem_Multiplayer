// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKSkillAbilitiy.h"
#include "HKGameplayTags.h"
#include "Character/HKCharacter.h"
#include "Player/HKPlayerController.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AbilityTask/SkillPartTask.h"

bool UHKSkillAbilitiy::GetLocalPlayerCondition(UHKAbilitySystemComponent* AbilitySystemComponent)
{
	if (bCloserMouseTarget)
	{
		AHKCharacter* PlayerCharacter = Cast<AHKCharacter>(AbilitySystemComponent->GetAvatarActor());
		AHKPlayerController* PlayerController = Cast<AHKPlayerController>(PlayerCharacter->GetLocalViewingPlayerController());
		AActor* MouseTarget = PlayerController->GetMousePointerActor();
		if (MouseTarget == nullptr)
		{
			if (bLockOn)
			{
				MouseTarget = PlayerController->GetLockOnTarget();
			}
			
			if (MouseTarget == nullptr)
				return false;
		}

		if (PlayerCharacter->GetDistanceTo(MouseTarget) <= CombatRange)
		{
			return true;
		}

		if (bLockOn)
		{
			PlayerController->LockOnTarget(MouseTarget, CombatRange - LockOnCloserRange, StartupInputTag);
		}
	}

	return false;
}

void UHKSkillAbilitiy::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	PartCount = 0;
	OnlyOne = false;
	if (!IsLocalPlayer())
	{
		CastSkill();
	}
	else
	{
		FindTargetDataUnderMouse();
	}
}

void UHKSkillAbilitiy::ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData)
{
	if (OnlyOne == true)
		return;
	Super::ActivateAbility_TargetDataUnderMouse(TargetData);
	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, 0);
	
	PlayMontage(Montage, EndTag, EndAbilityWhenCompleteMontage);
	FacingPosition(HitResult.Location);
	CastSkill();
	OnlyOne = true;
}

void UHKSkillAbilitiy::OccurMontageEvent(const AActor* TargetActor, const FVector& CombatSocketLocation)
{
	OnCompleteAbility(); //End Tag 발생시 종료
}

void UHKSkillAbilitiy::CastSkill()
{
	if (PartCount < SkillPartInfo.Num())
	{
		FSkillPartTaskInfo TaskInfo = SkillPartInfo[PartCount];
		USkillPartTask* NewTask = NewObject<USkillPartTask>(this, TaskInfo.Task);
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
