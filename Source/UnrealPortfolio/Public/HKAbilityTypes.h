#pragma once


#include "GameplayEffectTypes.h"
#include "HKAbilityTypes.generated.h"


USTRUCT(BlueprintType)
struct FHKGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsCriticalHit() const { return bIsCriticalHit; }
	bool IsMiss() const { return bIsMiss; }

	void SetIsCriticalHit(bool bInIsCriticalHit) { bIsCriticalHit = bInIsCriticalHit; }
	void SetIsMiss(bool bInIsMiss) { bIsMiss = bInIsMiss; }

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FHKGameplayEffectContext* Duplicate() const
	{
		FHKGameplayEffectContext* NewContext = new FHKGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;


protected:

	UPROPERTY()
	bool bIsMiss = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

};

template<>
struct TStructOpsTypeTraits<FHKGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FHKGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};