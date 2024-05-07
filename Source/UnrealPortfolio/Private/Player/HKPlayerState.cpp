// Copyright Druid Mechanics


#include "Player/HKPlayerState.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "AbilitySystem/HKAttributeSet.h"

AHKPlayerState::AHKPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UHKAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UHKAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AHKPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
