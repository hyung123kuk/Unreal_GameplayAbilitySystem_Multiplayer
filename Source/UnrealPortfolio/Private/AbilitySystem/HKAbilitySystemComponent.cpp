// Copyright Druid Mechanics


#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/HKGameplayAbility.h"
#include "Interaction/AbilityInterface.h"
#include "Kismet/GameplayStatics.h"
#include "HKGameplayTags.h"

void UHKAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UHKAbilitySystemComponent::ClientEffectApplied);
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	const bool bLocalCharacter = PlayerController == GetAvatarActor()->GetInstigatorController();
	//if (NM_Client == GetNetMode() || NM_Standalone == GetNetMode() || (NM_DedicatedServer == GetNetMode() && bLocalCharacter) )
	{
		GetWorld()->GetTimerManager().SetTimer(MainTimerHandle, this, &ThisClass::BroadCastCoolDown, CooldownBroadCastInterval, true);
	}
}

void UHKAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AddAbilities, FGameplayTag Tag)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : AddAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		if (const UHKGameplayAbility* HKAbility = Cast<UHKGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(HKAbility->StartupInputTag);

			if (IsOwnerActorAuthoritative())
				GiveAbility(AbilitySpec);

			if (AbilitySpecs.Contains(Tag))
			{
				TArray<FGameplayAbilitySpec> AbilitySpecArray = AbilitySpecs[Tag];
				AbilitySpecArray.Add(AbilitySpec);
				AbilitySpecs.Remove(Tag);
				AbilitySpecs.Add(Tag, AbilitySpecArray);
			}
			else
			{
				TArray<FGameplayAbilitySpec> AbilitySpecArray;
				AbilitySpecArray.Add(AbilitySpec);
				AbilitySpecs.Add(Tag, AbilitySpecArray);
			}
		}
	}
}

void UHKAbilitySystemComponent::RemoveAbilities(FGameplayTag Tag)
{
	if (AbilitySpecs.Contains(Tag))
	{
		const TArray<FGameplayAbilitySpec> AbilitySpecArray = AbilitySpecs[Tag];
		for (FGameplayAbilitySpec Spec : AbilitySpecArray)
		{
			if (IsOwnerActorAuthoritative())
				ClearAbility(Spec.Handle);
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

void UHKAbilitySystemComponent::BroadCastCoolDown()
{
	for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability->GetCooldownGameplayEffect() != nullptr)
		{
			FGameplayTag CooldownTag = AbilitySpec.Ability->GetCooldownGameplayEffect()->CachedGrantedTags.GetByIndex(0);
			float CoolDownRemain = AbilitySpec.Ability->GetCooldownTimeRemaining(AbilityActorInfo.Get());
			SkillCooldownDelegate.Broadcast(CooldownTag, CoolDownRemain > 0.1f, CoolDownRemain);
		}
	}
}

void UHKAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
