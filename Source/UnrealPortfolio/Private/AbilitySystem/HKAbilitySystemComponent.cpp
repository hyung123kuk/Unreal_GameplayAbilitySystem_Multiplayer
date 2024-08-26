// Copyright Druid Mechanics


#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/HKGameplayAbility.h"
#include "Interaction/AbilityInterface.h"

void UHKAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UHKAbilitySystemComponent::ClientEffectApplied);
}

void UHKAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AddAbilities, FGameplayTag Tag)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : AddAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UHKGameplayAbility* HKAbility = Cast<UHKGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(HKAbility->StartupInputTag);
			FGameplayAbilitySpecHandle SpecHandle = GiveAbility(AbilitySpec);
			if (AbilitySpecHandle.Contains(Tag))
			{
				TArray<FGameplayAbilitySpecHandle> SpecHandleArray = AbilitySpecHandle[Tag];
				SpecHandleArray.Add(SpecHandle);
				AbilitySpecHandle.Remove(Tag);
				AbilitySpecHandle.Add(Tag, SpecHandleArray);
			}
			else
			{
				TArray<FGameplayAbilitySpecHandle> SpecHandleArray;
				SpecHandleArray.Add(SpecHandle);
				AbilitySpecHandle.Add(Tag, SpecHandleArray);
			}
		}
	}
}

void UHKAbilitySystemComponent::RemoveAbilities(FGameplayTag Tag)
{
	if (AbilitySpecHandle.Contains(Tag))
	{
		TArray<FGameplayAbilitySpecHandle> SpecHandleArray = AbilitySpecHandle[Tag];
		for (FGameplayAbilitySpecHandle Handle : SpecHandleArray)
		{
			ClearAbility(Handle);
		}
	}
}

bool UHKAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) 
		return false;

	for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			IAbilityInterface* AbilityInterface = Cast<IAbilityInterface>(AbilitySpec.Ability);
			float CoolDownRemain = AbilitySpec.Ability->GetCooldownTimeRemaining(AbilityActorInfo.Get());
			bool Cost = AbilitySpec.Ability->CheckCost(AbilitySpec.Handle, AbilityActorInfo.Get());
			if (CoolDownRemain > 0.0f)
			{
				UE_LOG(LogTemp, Log, TEXT("Cool Down : %f"), CoolDownRemain);
				return true;
			}

			if (!Cost)
			{
				UE_LOG(LogTemp, Log, TEXT("Lack Of Cost"));
				return false;
			}

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
