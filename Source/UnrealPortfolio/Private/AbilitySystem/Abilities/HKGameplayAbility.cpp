// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKGameplayAbility.h"
#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"

void UHKGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	FindTargetDataUnderMouse();
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
		UTargetDataUnderMouse* TargetDataUnderMouseTask = UTargetDataUnderMouse::CreateTargetDataUnderMouse(this);
		TargetDataUnderMouseTask->ValidData.AddDynamic(this, &UHKGameplayAbility::ActivateAbility_TargetDataUnderMouse);
		TargetDataUnderMouseTask->ReadyForActivation();
	}
}

void UHKGameplayAbility::ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData)
{

}
