// Fill out your copyright notice in the Description page of Project Settings.


#include "HKGameplayTags.h"
#include "GameplayTagsManager.h"

FHKGameplayTags FHKGameplayTags::GameplayTags;

void FHKGameplayTags::InitializeNativeGameplayTags()
{
	//
	//Primary Attribute;
	//

	GameplayTags.Attribute_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Strength"),
		FString("Increases physical damage")
	);

	GameplayTags.Attribute_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Intelligence"),
		FString("Increases Magical damage")
	);

	GameplayTags.Attribute_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration")
	);

	GameplayTags.Attribute_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Primary.Vigor"),
		FString("Increases Health")
	);


	//
	//Secondary Attribute;
	//

	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.Armor"),
		FString("Reduces damage taken, improves Block Chance")
	);

	GameplayTags.Attribute_Secondary_MissChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MissChance"),
		FString("Chance to Miss")
	);

	GameplayTags.Attribute_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitChance"),
		FString("Chance to double damage plus critical hit bonus")
	);

	GameplayTags.Attribute_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.CriticalHitDamage"),
		FString("Bonus damage added when a critical hit is scored")
	);

	GameplayTags.Attribute_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.HealthRegeneration"),
		FString("Amount of Health regenerated every 1 second")
	);

	GameplayTags.Attribute_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.ManaRegeneration"),
		FString("Amount of Mana regenerated every 1 second")
	);

	GameplayTags.Attribute_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable")
	);

	GameplayTags.Attribute_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of Mana obtainable")
	);

	GameplayTags.Team_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Team.1"),
		FString("Team 1")
	);
	GameplayTags.Team_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Team.2"),
		FString("Team 2")
	);
	GameplayTags.Team_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Team.3"),
		FString("Team 3")
	);

	//
	//Input Tags;
	//

	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.LMB"),
		FString("Input Tag for Left Mouse Button")
	);

	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.RMB"),
		FString("Input Tag for Right Mouse Button")
	);

	GameplayTags.InputTag_Quick = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Quick"),
		FString("Input Tag for QuickSlot")
	);

	GameplayTags.InputTag_Quick_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Quick.1"),
		FString("Input Tag for 1 Key")
	);

	GameplayTags.InputTag_Quick_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Quick.2"),
		FString("Input Tag for 2 Key")
	);

	GameplayTags.InputTag_Quick_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Quick.3"),
		FString("Input Tag for 3 Key")
	);

	GameplayTags.InputTag_Quick_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Quick.4"),
		FString("Input Tag for 4 Key")
	);

	GameplayTags.InputTag_Quick_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Quick.5"),
		FString("Input Tag for 5 Key")
	);

	GameplayTags.InputTag_Quick_6 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Quick.6"),
		FString("Input Tag for 6 Key")
	);

	GameplayTags.InputTag_Quick_7 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Quick.7"),
		FString("Input Tag for 7 Key")
	);

	GameplayTags.InputTag_Quick_8 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.Quick.8"),
		FString("Input Tag for 8 Key")
	);

	GameplayTags.InputTag_K = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.K"),
		FString("Input Tag for K Key")
	);

	GameplayTags.InputTag_I = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("InputTag.I"),
		FString("Input Tag for I Key")
	);


	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Damage"),
		FString("Damage")
	);

	//
	// Effects
	//

	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag granted when Hit Reacting")
	);

	//
	// Abilities
	//

	GameplayTags.Abilities_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Attack"),
		FString("Attack Ability Tag")
	);

	GameplayTags.Abilities_Swap = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Abilities.Swap"),
		FString("Swap Ability Tag")
	);

	//
	// Combat Sockets
	//

	GameplayTags.CombatSocket_Character = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Character"),
		FString("Character Mesh")
	);

	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CombatSocket.Weapon"),
		FString("Weapon")
	);

	//
	// Montage Tags
	//

	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.1"),
		FString("Attack 1")
	);

	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.2"),
		FString("Attack 2")
	);

	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.3"),
		FString("Attack 3")
	);

	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Attack.4"),
		FString("Attack 4")
	);


	GameplayTags.Montage_Swap = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Montage.Swap"),
		FString("Swap Montage")
	);

	//
	// Attack Type
	//

	GameplayTags.AttackType_Melee = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AttackType.Melee"),
		FString("Melee Attack")
	);

	GameplayTags.AttackType_Projectile = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AttackType.Projectile"),
		FString("Projectile Attack")
	);

	GameplayTags.AttackType_Cast = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AttackType.Cast"),
		FString("Cast Attack")
	);


	//
	//	CoolDown
	//

	GameplayTags.CoolDown_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CoolDown.Attack"),
		FString("Attack CoolDown")
	);

	GameplayTags.CoolDown_SwapWeapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CoolDown.SwapWeapon"),
		FString("SwapWeapon CoolDown")
	);

	GameplayTags.CoolDown_5Combo = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CoolDown.5Combo"),
		FString("5Combo CoolDown")
	);

	GameplayTags.CoolDown_MultiShot = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CoolDown.MultiShot"),
		FString("MultiShot CoolDown")
	);

	GameplayTags.CoolDown_SuddenAttack = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("CoolDown.SuddenAttack"),
		FString("SuddenAttack CoolDown")
	);

	//
	//	Skiil
	//

	GameplayTags.Skill_End = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Skill.End"),
		FString("Skill End")
	);

	GameplayTags.Skill_5Combo_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Skill.5Combo.1"),
		FString("5Combo_1")
	);

	GameplayTags.Skill_5Combo_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Skill.5Combo.2"),
		FString("5Combo_2")
	);

	GameplayTags.Skill_5Combo_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Skill.5Combo.3"),
		FString("5Combo_3")
	);

	GameplayTags.Skill_5Combo_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Skill.5Combo.4"),
		FString("5Combo_4")
	);

	GameplayTags.Skill_5Combo_5 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Skill.5Combo.5"),
		FString("5Combo_5")
	);

	GameplayTags.Skill_MultiShot_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Skill.MultiShot.1"),
		FString("MultiShot_1")
	);

	GameplayTags.Skill_MultiShot_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Skill.MultiShot.2"),
		FString("MultiShot_2")
	);

	GameplayTags.Skill_SuddenAttack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Skill.SuddenAttack.1"),
		FString("SuddenAttack_1")
	);

	//
	//	Act Type
	//

	GameplayTags.Act_SwapType_Sword = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Act.SwapType.Sword"),
		FString("Sword Swap")
	);

	GameplayTags.Act_SwapType_Bow = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Act.SwapType.Bow"),
		FString("Bow Swap")
	);

	GameplayTags.Act_SwapType_Staff = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Act.SwapType.Staff"),
		FString("Staff Swap")
	);

	//
	// EquipmentType
	//

	GameplayTags.EquipmentType_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("EquipmentType.Weapon"),
		FString("EquipmentType")
	);

	//
	// TriggerType
	//

	GameplayTags.Trigger_UseItem = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Trigger.UseItem"),
		FString("Use Item")
	);


	//
	// GameplayCue
	//


	GameplayTags.GameplayCue_MeleeImpact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("GameplayCue.MeleeImpact"),
		FString("MeleeImpact")
	);

	//
	//	AbilityType
	//

	GameplayTags.AbilityType_Skill = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AbilityType.Skill"),
		FString("Skill Ability")
	);

	GameplayTags.AbilityType_Weapoon = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AbilityType.Weapoon"),
		FString("Weapon Ability")
	);

	GameplayTags.AbilityType_StartUp = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("AbilityType.StartUp"),
		FString("StartUp Ability")
	);

}
