// Copyright Druid Mechanics

#include "Skill/SkillInventory.h"
#include "Abilities/GameplayAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HKPlayerController.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "Skill/SkillInfoData.h"
#include "HKGameplayTags.h"
#include "AbilitySystem/Abilities/HKGameplayAbility.h"

void USkillInventory::Init(UHKAbilitySystemComponent* INASC)
{
	ASC = INASC;

	for (FSkillInfo Skill : Skills)
	{
		AddSkillDelegate.Broadcast(Skill);
	}
}

void USkillInventory::AddSkillInventoryAbilities(UHKAbilitySystemComponent* INASC)
{
	ASC = INASC;

	TArray<TSubclassOf<UGameplayAbility>> SkillAbilities;
	for (FSkillInfo Skill : Skills)
	{
		SkillAbilities.Add(Skill.SkillAbility);
	}

	ASC->AddCharacterAbilities(SkillAbilities, FHKGameplayTags::Get().AbilityType_Skill);
}

FSkillInfo USkillInventory::FindSkill(const int Id)
{
	for (FSkillInfo Skill : Skills)
	{
		if (Skill.Id == Id)
		{
			return Skill;
		}
	}

	return FSkillInfo();
}

void USkillInventory::PressedSkill(const int Id)
{
	for (FSkillInfo Skill : Skills)
	{
		if (Skill.Id == Id)
		{
			UHKGameplayAbility* HKGameplayAbility = Cast<UHKGameplayAbility>(Skill.SkillAbility.GetDefaultObject());
			ASC->AbilityInputTagHeld(HKGameplayAbility->StartupInputTag);
			PressedSkillDelegate.Broadcast(Skill);
		}
	}
}

void USkillInventory::HeldSkill(const int Id)
{
	for (FSkillInfo Skill : Skills)
	{
		if (Skill.Id == Id)
		{
			UHKGameplayAbility* HKGameplayAbility = Cast<UHKGameplayAbility>(Skill.SkillAbility.GetDefaultObject());
			ASC->AbilityInputTagHeld(HKGameplayAbility->StartupInputTag);
			HeldSkillDelegate.Broadcast(Skill);
		}
	}
}

void USkillInventory::ReleasedSkill(const int Id)
{
	for (FSkillInfo Skill : Skills)
	{
		if (Skill.Id == Id)
		{
			UHKGameplayAbility* HKGameplayAbility = Cast<UHKGameplayAbility>(Skill.SkillAbility.GetDefaultObject());
			ASC->AbilityInputTagReleased(HKGameplayAbility->StartupInputTag);
			ReleasedSkillDelegate.Broadcast(Skill);
		}
	}
}
