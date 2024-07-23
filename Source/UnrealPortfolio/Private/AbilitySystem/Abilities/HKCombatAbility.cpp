// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKCombatAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HKGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/HKAbilitySystemLibrary.h"
#include "Player/HKPlayerController.h"
#include "Interaction/MouseTargetActorInterface.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "Character/HKCharacter.h"
#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"


void UHKCombatAbility::CauseDamage(AActor* TargetActor, float Damage)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, FHKGameplayTags::Get().Damage, Damage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

bool UHKCombatAbility::GetLocalPlayerCondition(UHKAbilitySystemComponent* AbilitySystemComponent)
{
	if (bEndAbilityOutOfCombatRange)
	{
		AHKCharacter* PlayerCharacter = Cast<AHKCharacter>(AbilitySystemComponent->GetAvatarActor());
		AHKPlayerController* PlayerController = Cast<AHKPlayerController>(PlayerCharacter->GetLocalViewingPlayerController());
		AActor* ClickMouseTarget = PlayerController->GetLastTargetActor();
		if (ClickMouseTarget == nullptr)
		{
			return false;
		}

		if (PlayerCharacter->GetDistanceTo(ClickMouseTarget) > CombatRange)
		{
			return false;
		}
	}

	return true;
}

void UHKCombatAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ActorCombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	Team = ActorCombatInterface->GetTeam();
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHKCombatAbility::FindTargetDataUnderMouse()
{
	if (IsLocalPlayer())
	{
		TArray<AActor*> TargetArray;
		AHKCharacter* PlayerCharacter = Cast<AHKCharacter>(GetAvatarActorFromActorInfo());
		AHKPlayerController* PlayerController = Cast<AHKPlayerController>(PlayerCharacter->GetController());
		AActor* ClickMouseTarget = PlayerController->GetLastTargetActor();
		TargetArray.Add(ClickMouseTarget);


		UTargetDataUnderMouse* TargetDataUnderMouseTask = UTargetDataUnderMouse::CreateTargetDataUnderMouse(this, TargetArray);
		TargetDataUnderMouseTask->ValidData.AddDynamic(this, &UHKCombatAbility::ActivateAbility_TargetDataUnderMouse);
		TargetDataUnderMouseTask->ReadyForActivation();
	}
}

bool UHKCombatAbility::IsSameTeam(const AActor* Actor,const AActor* Actor2)
{
	return UHKAbilitySystemLibrary::IsSameTeam(Actor,Actor2);
}

bool UHKCombatAbility::PlayRandomAttackMontage(FGameplayTag AttackType)
{
	TArray<FTaggedMontage> TypeMontages;
	for (FTaggedMontage& Montage : ActorCombatInterface->GetAttackMontages())
	{
		if (Montage.AttackType == AttackType)
			TypeMontages.Add(Montage);
	}

	TaggedMontage = GetRandomTaggedMontageFromArray(TypeMontages);
	if (TaggedMontage.Montage == nullptr)
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return false;
	}

	PlayMontage(TaggedMontage.Montage, TaggedMontage.MontageTag);
	return true;
}

FTaggedMontage UHKCombatAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}

	return FTaggedMontage();
}

void UHKCombatAbility::FacingTarget()
{
	Target = ActorCombatInterface->GetCombatTarget();
	if (Target != nullptr)
	{
		ActorCombatInterface->Execute_UpdateFacingTarget(GetAvatarActorFromActorInfo(), Target->GetActorLocation());
	}
}

void UHKCombatAbility::PlayMontage(UAnimMontage* MontageToPlay, FGameplayTag MontageEvent)
{
	UE_LOG(LogTemp, Log, TEXT("%s"), *MontageEvent.GetTagName().ToString());

	UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, MontageEvent);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &UHKCombatAbility::OnOccurMontageEvent);

	WaitGameplayEventTask->ReadyForActivation();
	
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), MontageToPlay);

	PlayAttackTask->OnCompleted.AddDynamic(this, &UHKCombatAbility::OnCompleteMontage);
	PlayAttackTask->OnCancelled.AddDynamic(this, &UHKCombatAbility::OnCancelledMontage);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UHKCombatAbility::OnInterruptedMontage);

	PlayAttackTask->ReadyForActivation();

}

void UHKCombatAbility::OccurMontageEvent(const AActor* TargetActor, const FVector& CombatSocketLocation)
{
}

void UHKCombatAbility::OnOccurMontageEvent(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Log, TEXT("OccurMontageEvent"));
	const FVector CombatSocketLocation = ActorCombatInterface->GetCombatSocketLocation(TaggedMontage.SocketTag, TaggedMontage.SocketName);
	OccurMontageEvent(Target, CombatSocketLocation);
}

void UHKCombatAbility::OnCompleteMontage()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHKCombatAbility::OnCancelledMontage()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHKCombatAbility::OnInterruptedMontage()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

