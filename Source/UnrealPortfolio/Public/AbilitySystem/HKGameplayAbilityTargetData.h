// Copyright Druid Mechanics

#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "UObject/Class.h"
#include "HKGameplayAbilityTargetData.generated.h"

class FArchive;
struct FGameplayEffectContextHandle;

USTRUCT()
struct FHKGameplayAbilityTargetData_Item : public FGameplayAbilityTargetData
{
	GENERATED_BODY()
	
	FHKGameplayAbilityTargetData_Item()
		: ID(-1), UniqueID(0)
	{ }

	UPROPERTY()
	int32 ID;

	UPROPERTY()
	int32 UniqueID;

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FHKGameplayAbilityTargetData_Item::StaticStruct();
	}
	
};

template<>
struct TStructOpsTypeTraits<FHKGameplayAbilityTargetData_Item> : public TStructOpsTypeTraitsBase2<FHKGameplayAbilityTargetData_Item>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};
