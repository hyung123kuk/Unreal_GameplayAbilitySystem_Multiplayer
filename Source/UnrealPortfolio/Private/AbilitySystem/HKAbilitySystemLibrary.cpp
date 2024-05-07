// Copyright Druid Mechanics


#include "AbilitySystem/HKAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "HKAbilityTypes.h"

bool UHKAbilitySystemLibrary::IsMiss(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHKGameplayEffectContext* AuraEffectContext = static_cast<const FHKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsMiss();
	}
	return false;
}

bool UHKAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHKGameplayEffectContext* AuraEffectContext = static_cast<const FHKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UHKAbilitySystemLibrary::SetIsMiss(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsMiss)
{
	if (FHKGameplayEffectContext* HKEffectContext = static_cast<FHKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HKEffectContext->SetIsMiss(bInIsMiss);
	}
}

void UHKAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FHKGameplayEffectContext* HKEffectContext = static_cast<FHKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HKEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}

}