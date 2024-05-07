// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "HKAbilitySystemGlobals.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
