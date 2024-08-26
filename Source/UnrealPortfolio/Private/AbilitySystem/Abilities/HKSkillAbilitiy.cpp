// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKSkillAbilitiy.h"
#include "HKGameplayTags.h"
#include "Character/HKCharacter.h"
#include "Player/HKPlayerController.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AbilityTask/SkillPartTask.h"
#include "Interaction/CombatInterface.h"

bool UHKSkillAbilitiy::GetLocalPlayerCondition(UHKAbilitySystemComponent* AbilitySystemComponent)
{
	if (bCloserMouseTarget)
	{
		AHKCharacter* PlayerCharacter = Cast<AHKCharacter>(AbilitySystemComponent->GetAvatarActor());
		AHKPlayerController* PlayerController = Cast<AHKPlayerController>(PlayerCharacter->GetLocalViewingPlayerController());
		FHitResult MouseTarget = PlayerController->GetNowMousePointer();

		if (MouseTarget.bBlockingHit && (MouseTarget.GetActor() == nullptr || !MouseTarget.GetActor()->Implements<UCombatInterface>()))
		{
			if (bLockOn)
			{
				MouseTarget = PlayerController->GetLockOnHitResult();
			}
		}

		if (!MouseTarget.bBlockingHit)
			return false;

		if (MouseTarget.GetActor() == nullptr)
			return false;

		if (!MouseTarget.GetActor()->Implements<UCombatInterface>())
			return false;


		if (PlayerCharacter->GetDistanceTo(MouseTarget.GetActor()) <= CombatRange)
		{
			return true;
		}

		if (bLockOn)
		{
			PlayerController->LockOnTarget(MouseTarget, CombatRange - LockOnCloserRange, StartupInputTag);
		}
		return false;
	}

	return true;
}

void UHKSkillAbilitiy::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	PartCount = 0;


	if (!IsLocalPlayer())
	{
		CastSkill();
	}
	else
	{
		AHKCharacter* PlayerCharacter = Cast<AHKCharacter>(GetAvatarActorFromActorInfo());
		AHKPlayerController* PlayerController = Cast<AHKPlayerController>(PlayerCharacter->GetLocalViewingPlayerController());

		FindTargetDataUnderMouse();
	}
}

void UHKSkillAbilitiy::ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData, const FGameplayTag& ActivationTag)
{
	Super::ActivateAbility_TargetDataUnderMouse(TargetData, ActivationTag);
	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, 0);

	if (HitResult.bBlockingHit && HitResult.GetActor() != nullptr)
		MouseTargetAtSkiilStartUp = HitResult.GetActor();

	PlayMontage(Montage, EndTag, EndAbilityWhenCompleteMontage);
	if (bFacingMousePositionWhenMotionWarping)
		FacingPosition(HitResult.Location);
	CastSkill();
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
		TaskInfo.PredictionKeyCurrent = PartCount + 1;
		TaskInfo.MouseTargetAtSkiilStartUp = MouseTargetAtSkiilStartUp;
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
