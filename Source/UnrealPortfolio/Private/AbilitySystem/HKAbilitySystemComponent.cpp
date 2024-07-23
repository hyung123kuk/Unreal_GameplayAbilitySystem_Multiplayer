// Copyright Druid Mechanics


#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/HKGameplayAbility.h"
#include "Interaction/AbilityInterface.h"

void UHKAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UHKAbilitySystemComponent::ClientEffectApplied);
}

void UHKAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UHKGameplayAbility* HKAbility = Cast<UHKGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(HKAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}
}

bool UHKAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) 
		return false;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			IAbilityInterface* AbilityInterface = Cast<IAbilityInterface>(AbilitySpec.Ability);
			if (AbilityInterface != nullptr)
			{
				if (!AbilityInterface->GetLocalPlayerCondition(this))
					continue;
			}

			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				if (TryActivateAbility(AbilitySpec.Handle))
					return true;
			}
			else
			{
				return true;
			}
		}
	}

	return false;
}

void UHKAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			if (AbilitySpec.IsActive())
			{
				AbilitySpecInputReleased(AbilitySpec);
			}
		}
	}
}

void UHKAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
