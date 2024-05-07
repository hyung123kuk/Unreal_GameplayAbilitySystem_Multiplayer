// Copyright Druid Mechanics


#include "Character/HKCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"

AHKCharacterBase::AHKCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

}

UAbilitySystemComponent* AHKCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHKCharacterBase::Die()
{
}

void AHKCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHKCharacterBase::InitAbilityActorInfo()
{
}

void AHKCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AHKCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AHKCharacterBase::AddCharacterAbilities()
{
	UHKAbilitySystemComponent* HKASC = CastChecked<UHKAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	HKASC->AddCharacterAbilities(StartupAbilities);
}

