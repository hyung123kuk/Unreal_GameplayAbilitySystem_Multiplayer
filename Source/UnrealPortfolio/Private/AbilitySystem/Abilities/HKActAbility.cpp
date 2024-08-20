// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKActAbility.h"
#include "HKGameplayTags.h"



void UHKActAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbilityCooldown(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
	PlayRandomActMontage(K2_GetActTagType());
}

void UHKActAbility::OnOccurMontageEvent(FGameplayEventData Payload)
{
	K2_OnOccurMontageEvent();
	OnCompleteAbility();
}
