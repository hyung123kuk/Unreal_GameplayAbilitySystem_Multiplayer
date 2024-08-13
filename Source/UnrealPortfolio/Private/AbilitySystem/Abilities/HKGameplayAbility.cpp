// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKGameplayAbility.h"
#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Character/HKCharacterBase.h"



void UHKGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CharacterBase = Cast<AHKCharacterBase>(GetAvatarActorFromActorInfo());
}

bool UHKGameplayAbility::IsLocalPlayer()
{
	APlayerController* PC = GetCurrentActorInfo()->PlayerController.Get();
	if (PC == nullptr)
	{
		return false;
	}
	return true;
}

void UHKGameplayAbility::FindTargetDataUnderMouse()
{
	if (IsLocalPlayer())
	{
		TArray<AActor*> TargetData;
		UTargetDataUnderMouse* TargetDataUnderMouseTask = UTargetDataUnderMouse::CreateTargetDataUnderMouse(this, TargetData);
		TargetDataUnderMouseTask->ValidData.AddDynamic(this, &UHKGameplayAbility::ActivateAbility_TargetDataUnderMouse);
		TargetDataUnderMouseTask->ReadyForActivation();
	}
}

void UHKGameplayAbility::ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData)
{

}


void UHKGameplayAbility::PlayRandomActMontage(FGameplayTag Type)
{
	TArray<FTaggedMontage> TypeMontages;
	for (FTaggedMontage& Montage : CharacterBase->GetActMontages())
	{
		if (Montage.Type == Type)
			TypeMontages.Add(Montage);
	}

	TaggedMontage = GetRandomTaggedMontageFromArray(TypeMontages);
	if (TaggedMontage.Montage == nullptr)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	PlayMontage(TaggedMontage.Montage, TaggedMontage.MontageTag);
}

FTaggedMontage UHKGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}

	return FTaggedMontage();
}

void UHKGameplayAbility::PlayMontage(UAnimMontage* MontageToPlay, FGameplayTag MontageEvent, bool EndAbilityWhenCompleteMontage)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *MontageEvent.GetTagName().ToString());

	UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, MontageEvent);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &UHKGameplayAbility::OnOccurMontageEvent);

	WaitGameplayEventTask->ReadyForActivation();

	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), MontageToPlay);
	if (EndAbilityWhenCompleteMontage)
	{
		PlayAttackTask->OnCompleted.AddDynamic(this, &UHKGameplayAbility::OnCompleteMontage);
	}

	PlayAttackTask->OnCancelled.AddDynamic(this, &UHKGameplayAbility::OnCancelledMontage);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UHKGameplayAbility::OnInterruptedMontage);
	PlayAttackTask->ReadyForActivation();
}

void UHKGameplayAbility::OnOccurMontageEvent(FGameplayEventData Payload)
{

}

void UHKGameplayAbility::OnCompleteMontage()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHKGameplayAbility::OnCancelledMontage()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHKGameplayAbility::OnInterruptedMontage()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHKGameplayAbility::OnCompleteAbility()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHKGameplayAbility::OnFailedAbility()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}
