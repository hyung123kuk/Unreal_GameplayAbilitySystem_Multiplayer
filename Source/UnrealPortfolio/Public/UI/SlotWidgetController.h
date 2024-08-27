// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKWidgetControllerBase.h"
#include "UI/Widget/HKSlotWidget.h"
#include "GameplayTagContainer.h"
#include "SlotWidgetController.generated.h"


USTRUCT(BlueprintType)
struct FSlotInfoWidgetControllerParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Id;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int UniqueId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHKSlotWidget* OriginSlotWitdet; // 처음 만들어 졌던 SlotWidget

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHKSlotWidget* SlotWitdet; //현재 위치 SlotWidget

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SlotNumber;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> SlotImage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESlotContainInformation ContainInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag CooldownTag;

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
