// Copyright Druid Mechanics

#include "Skill/SkillInventory.h"
#include "Abilities/GameplayAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HKPlayerController.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "Skill/SkillInfoData.h"
#include "AbilitySystem/Abilities/HKGameplayAbility.h"

void USkillInventory::Init(UHKAbilitySystemComponent* INASC)
{
	ASC = INASC;

	GetWorld()->GetTimerManager().SetTimer(CoolTimeTimerHandle, this, &ThisClass::OnCoolTimeTimer, TimerTickInterval, true);
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

	ASC->AddCharacterAbilities(SkillAbilities);
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

void USkillInventory::TryCastSkill(const int Id)
{
	for (FSkillInfo Skill : Skills)
	{
		if (Skill.Id == Id)
		{
			UHKGameplayAbility* HKGameplayAbility = Cast<UHKGameplayAbility>(Skill.SkillAbility.GetDefaultObject());
			ASC->AbilityInputTagHeld(HKGameplayAbility->StartupInputTag);
			CastSkillDelegate.Broadcast(Skill);
		}
	}
}

void USkillInventory::OnCoolTimeTimer()
{
	for (FSkillInfo Skill : Skills)
	{
		SkillCoolTimeDelegate.Broadcast(Skill,0.0f);
	}
}
