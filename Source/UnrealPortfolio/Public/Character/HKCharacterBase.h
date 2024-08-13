// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "CharacterClassInfo.h"
#include "GameplayTagContainer.h"
#include "Item/Inventory.h"
#include "Skill/SkillInventory.h"
#include "Skill/SkillInfoData.h"
#include "HKCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class UGameplayAbility;
class UGameplayEffect;
struct FUserItem;

UCLASS(Abstract)
class UNREALPORTFOLIO_API AHKCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AHKCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/** Combat Interface */
	virtual void Die() override;
	virtual bool IsDead() const override;
	virtual void SetCombatTarget(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget() const override;
	virtual TArray<FTaggedMontage> GetAttackMontages() override;
	virtual FVector GetCombatSocketLocation(const FGameplayTag& SocketTag,const FName& SocketName);
	virtual const FGameplayTag& GetTeam() const;
	/**Combat Interface End*/

	/** Act **/
	UFUNCTION(BlueprintCallable)
	FUserItem GetEquipItem(FGameplayTag EquipTag);
	TArray<FTaggedMontage> GetActMontages();

	UFUNCTION(BlueprintCallable)
	void UseItem(FUserItem Item);

	UFUNCTION(BlueprintCallable)
	void SetEquipItem(FUserItem NewItem);

	TMap<FGameplayTag, FUserItem> EquipmentItem;
	/** Act **/

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	USkillInventory* GetSkillInventory() { return SkillInventory; }
	
	UFUNCTION(BlueprintCallable)
	void CastSkill(int SkillId);

	void MakeSkillInventory();
	void InitSkillInventory();

protected:
	void OccurGameplayTags(FGameplayTag GameplayTags);
	virtual void InitAbilityActorInfo();


	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities);
	void RemoveCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities);


	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<FTaggedMontage> AttackMontage;

	UPROPERTY(EditAnywhere, Category = "Montages")
	TArray<FTaggedMontage> ActMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* DeathSound;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	bool bDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	bool bMeleeAttack = true;

	UPROPERTY(VisibleAnywhere)
	FGameplayTag Team;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(EditAnywhere, Category = "Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;


	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<USkillInventory> SkillInventoryClass;

	UPROPERTY()
	TObjectPtr<USkillInventory> SkillInventory;

};
