// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SkillInfoData.h"
#include "SkillInventory.generated.h"

class UHKAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkiilDelegate, FSkillInfo, SkillInfo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkiilCoolTimeDelegate, FSkillInfo, SkillInfo, float, RemainTime);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API USkillInventory : public UObject
{
	GENERATED_BODY()
	
	friend class AHKCharacterBase;
	
public:
	void Init(UHKAbilitySystemComponent* INASC);
	void AddSkillInventoryAbilities(UHKAbilitySystemComponent* INASC);

	UPROPERTY(BlueprintAssignable)
	FSkiilDelegate AddSkillDelegate;

	UPROPERTY(BlueprintAssignable)
	FSkiilCoolTimeDelegate SkillCoolTimeDelegate;

	UPROPERTY(BlueprintAssignable)
	FSkiilDelegate PressedSkillDelegate;

	UPROPERTY(BlueprintAssignable)
	FSkiilDelegate HeldSkillDelegate;

	UPROPERTY(BlueprintAssignable)
	FSkiilDelegate ReleasedSkillDelegate;
	
public:
	UFUNCTION(BlueprintCallable)
	FSkillInfo FindSkill(const int Id);

	UFUNCTION(BlueprintCallable)
	void PressedSkill(const int Id);

	UFUNCTION(BlueprintCallable)
	void HeldSkill(const int Id);

	UFUNCTION(BlueprintCallable)
	void ReleasedSkill(const int Id);

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Skills")
	TArray<FSkillInfo> Skills;

	FTimerHandle CoolTimeTimerHandle;

	UPROPERTY()
	TObjectPtr<UHKAbilitySystemComponent> ASC;

	UPROPERTY()
	float TimerTickInterval = 0.1f;

	UFUNCTION()
	void OnCoolTimeTimer();
};
