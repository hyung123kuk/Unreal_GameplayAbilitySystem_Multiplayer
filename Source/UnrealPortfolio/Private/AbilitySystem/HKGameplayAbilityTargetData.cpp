// Copyright Druid Mechanics


#include "AbilitySystem/HKGameplayAbilityTargetData.h"

#include "Serialization/Archive.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(HKGameplayAbilityTargetData)

struct FGameplayEffectContextHandle;

bool FHKGameplayAbilityTargetData_Item::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	Ar << ID;
	Ar << UniqueID;

	return true;
}