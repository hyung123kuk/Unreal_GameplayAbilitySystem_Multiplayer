// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKAttackAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "HKGameplayTags.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

void UHKAttackAbility::CauseDamage(AActor* TargetActor, float Damage)
{
	FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, FHKGameplayTags::Get().Damage, Damage);
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
}


bool UHKAttackAbility::PlayRandomAttackMontage(FGameplayTag AttackType)
{
	ICombatInterface* Combat = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	TArray<FTaggedMontage> TypeMontages;
	for (FTaggedMontage& Montage : Combat->GetAttackMontages())
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

FTaggedMontage UHKAttackAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}

	return FTaggedMontage();
}

void UHKAttackAbility::FacingTarget()
{
	ICombatInterface* Combat = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	AActor* Target = Combat->GetCombatTarget();
	Combat->UpdateFacingTarget(Target->GetActorLocation());
}

void UHKAttackAbility::PlayMontage(UAnimMontage* MontageToPlay, FGameplayTag MontageEvent, float Rate, FName StartSection)
{
	UAbilityTask_PlayMontageAndWait* PlayAttackTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, TEXT("PlayAttack"), MontageToPlay, Rate, StartSection);
	PlayAttackTask->OnCompleted.AddDynamic(this, &UHKAttackAbility::OnCompleteMontage);
	PlayAttackTask->OnCancelled.AddDynamic(this, &UHKAttackAbility::OnCancelledMontage);
	PlayAttackTask->OnInterrupted.AddDynamic(this, &UHKAttackAbility::OnInterruptedMontage);
	PlayAttackTask->ReadyForActivation();

	UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, MontageEvent);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &UHKAttackAbility::OccurMontageEvent);
	WaitGameplayEventTask->ReadyForActivation();
}

void UHKAttackAbility::OccurMontageEvent(const AActor* AvatarActor, const FVector& CombatSocketLocation)
{
}

void UHKAttackAbility::OccurMontageEvent(FGameplayEventData Payload)
{
	ICombatInterface* Combat = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	const FVector CombatSocketLocation = Combat->GetCombatSocketLocation(TaggedMontage.SocketTag, TaggedMontage.SocketName);
	OccurMontageEvent(GetAvatarActorFromActorInfo(), CombatSocketLocation);
}

void UHKAttackAbility::OnCompleteMontage()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = false;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHKAttackAbility::OnCancelledMontage()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UHKAttackAbility::OnInterruptedMontage()
{
	bool bReplicateEndAbility = true;
	bool bWasCancelled = true;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, bReplicateEndAbility, bWasCancelled);
}

