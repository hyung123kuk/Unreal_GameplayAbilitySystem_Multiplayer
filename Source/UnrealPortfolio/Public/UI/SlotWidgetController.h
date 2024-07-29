// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKWidgetControllerBase.h"
#include "UI/Widget/HKSlotWidget.h"
#include "SlotWidgetController.generated.h"

class UHKGameplayAbility;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FSlotInfoWidgetControllerParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHKSlotWidget* OriginSlotWitdet; // 처음 만들어 졌던 SlotWidget

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SlotNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> SlotImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlotContainInformation ContainInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UHKGameplayAbility*> GameAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UGameplayEffect*> GameplayEffects;
	
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API USlotWidgetController : public UHKWidgetControllerBase
{
	GENERATED_BODY()
	
public:
	void SetWidgetControllerParams(const FSlotInfoWidgetControllerParams& Params);
	
	FSlotInfoWidgetControllerParams GetSlotInfoParmas() { return SlotInfoParams; }
	ESlotContainInformation GetContainInfo() { return SlotInfoParams.ContainInfo; }

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	FSlotInfoWidgetControllerParams SlotInfoParams;

};
