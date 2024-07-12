// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKCombatAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HKGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/HKAbilitySystemLibrary.h"

void UHKCombatAbility::CauseDamage(AActor* TargetActor, float Damage)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, FHKGameplayTags::Get().Damage, Damage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}

void UHKCombatAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	ActorCombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	Team = ActorCombatInterface->GetTeam();
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
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
	AActor* Target = ActorCombatInterface->GetCombatTarget();
	if (Target != nullptr)
	{
		ActorCombatInterface->Execute_UpdateFacingTarget(GetAvatarActorFromActorInfo(), Target->GetActorLocation());
	}
}

void UHKCombatAbility::PlayMontage(UAnimMontage* MontageToPlay, FGameplayTag MontageEvent, float Rate, FName StartSection)
{
	UE_LOG(LogTemp, Log, TEXT("PlayeMontage"));
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), MontageToPlay, Rate, StartSection);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UHKCombatAbility::OnCompleteMontage);
	PlayAttackTask->OnCancelled.AddDynamic(this, &UHKCombatAbility::OnCancelledMontage);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UHKCombatAbility::OnInterruptedMontage);
	PlayAttackTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, MontageEvent);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &UHKCombatAbility::OccurMontageEvent);
	WaitGameplayEventTask->ReadyForActivation();
}

void UHKCombatAbility::OccurMontageEvent(const AActor* AvatarActor, const FVector& CombatSocketLocation)
{
}

void UHKCombatAbility::OccurMontageEvent(FGameplayEventData Payload)
{
	const FVector CombatSocketLocation = ActorCombatInterface->GetCombatSocketLocation(TaggedMontage.SocketTag, TaggedMontage.SocketName);
	OccurMontageEvent(GetAvatarActorFromActorInfo(), CombatSocketLocation);
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

