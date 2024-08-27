// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "HKAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FCooldownSignature, FGameplayTag, CooldonwTag, bool, bCoolDown, float, RemainTime);

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	void AbilityActorInfoSet();

	FEffectAssetTags EffectAssetTags;


	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AddAbilities, FGameplayTag Tag);
	void RemoveAbilities(FGameplayTag TypeTag);

	bool AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	UFUNCTION()
	void BroadCastCoolDown();

	TMap<FGameplayTag, TArray<FGameplayAbilitySpec>> AbilitySpecs;
	
	UPROPERTY()
	FCooldownSignature SkillCooldownDelegate;

protected:
	UFUNCTION(Client, Reliable)
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle);

	FTimerHandle MainTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAGameMode")
	float CooldownBroadCastInterval = 0.03f;
	
};
