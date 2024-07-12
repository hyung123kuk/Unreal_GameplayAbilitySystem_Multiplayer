// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

struct FHKGameplayTags
{
public:
	static const FHKGameplayTags& Get() { return  GameplayTags; }
	static void InitializeNativeGameplayTags();

	//
	//	Primary Attribute;
	//
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;

	//
	//	Secondary Attribute;
	//
	FGameplayTag Attribute_Secondary_Armor;
	FGameplayTag Attribute_Secondary_MissChance;
	FGameplayTag Attribute_Secondary_CriticalHitChance;
	FGameplayTag Attribute_Secondary_CriticalHitDamage;
	FGameplayTag Attribute_Secondary_HealthRegeneration;
	FGameplayTag Attribute_Secondary_ManaRegeneration;
	FGameplayTag Attribute_Secondary_MaxHealth;
	FGameplayTag Attribute_Secondary_MaxMana;

	FGameplayTag Team_1;
	FGameplayTag Team_2;
	FGameplayTag Team_3;

	//
	//Input Tags;
	//
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_1;
	FGameplayTag InputTag_2;
	FGameplayTag InputTag_3;
	FGameplayTag InputTag_4;

	FGameplayTag Damage;

	FGameplayTag Abilities_Attack;

	FGameplayTag CombatSocket_Character;
	FGameplayTag CombatSocket_Weapon;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;

	FGameplayTag AttackType_Melee;
	FGameplayTag AttackType_Projectile;
	FGameplayTag AttackType_Cast;

	FGameplayTag Effects_HitReact;

	FGameplayTag GameplayCue_MeleeImpact;

private:
	static FHKGameplayTags GameplayTags;

};