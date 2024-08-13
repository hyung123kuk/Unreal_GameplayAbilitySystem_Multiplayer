// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UI/Widget/HKSlotWidget.h"
#include "SkillInfoData.generated.h"

class UGameplayAbility;


USTRUCT(BlueprintType)
struct FSkillInfo : public FSlotStruct
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Abilities")
	TSubclassOf<UGameplayAbility> SkillAbility;

};
