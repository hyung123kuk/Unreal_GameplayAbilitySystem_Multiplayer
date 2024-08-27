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
	//	Primary Attribute
	//
	FGameplayTag Attribute_Primary_Strength;
	FGameplayTag Attribute_Primary_Intelligence;
	FGameplayTag Attribute_Primary_Resilience;
	FGameplayTag Attribute_Primary_Vigor;

	//
	//	Secondary Attribute
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
	//	Input Tags
	//
	FGameplayTag InputTag_LMB;
	FGameplayTag InputTag_RMB;
	FGameplayTag InputTag_K;
	FGameplayTag InputTag_I;

	FGameplayTag InputTag_Quick;
	FGameplayTag InputTag_Quick_1;
	FGameplayTag InputTag_Quick_2;
	FGameplayTag InputTag_Quick_3;
	FGameplayTag InputTag_Quick_4;
	FGameplayTag InputTag_Quick_5;
	FGameplayTag InputTag_Quick_6;
	FGameplayTag InputTag_Quick_7;
	FGameplayTag InputTag_Quick_8;

	
	FGameplayTag Damage;

	FGameplayTag Abilities_Attack;
	FGameplayTag Abilities_Swap;

	FGameplayTag CombatSocket_Character;
	FGameplayTag CombatSocket_Weapon;

	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	FGameplayTag Montage_Swap;

	FGameplayTag AttackType_Melee;
	FGameplayTag AttackType_Projectile;
	FGameplayTag AttackType_Cast;
	FGameplayTag Act_SwapType_Sword;
	FGameplayTag Act_SwapType_Bow;
	FGameplayTag Act_SwapType_Staff;

	//CoolDown
	FGameplayTag CoolDown_Attack;
	FGameplayTag CoolDown_SwapWeapon;

	FGameplayTag CoolDown_5Combo;
	FGameplayTag CoolDown_MultiShot;
	FGameplayTag CoolDown_SuddenAttack;

	//Skiil
	FGameplayTag Skill_End;

	FGameplayTag Skill_5Combo_1;
	FGameplayTag Skill_5Combo_2;
	FGameplayTag Skill_5Combo_3;
	FGameplayTag Skill_5Combo_4;
	FGameplayTag Skill_5Combo_5;

	FGameplayTag Skill_MultiShot_1;
	FGameplayTag Skill_MultiShot_2;

	FGameplayTag Skill_SuddenAttack_1;

	
	FGameplayTag AbilityType_Skill;
	FGameplayTag AbilityType_Weapoon;
	FGameplayTag AbilityType_StartUp;
	
	FGameplayTag EquipmentType_Weapon;

	FGameplayTag Trigger_UseItem;

	FGameplayTag Effects_HitReact;


	FGameplayTag GameplayCue_MeleeImpact;

private:
	static FHKGameplayTags GameplayTags;

};