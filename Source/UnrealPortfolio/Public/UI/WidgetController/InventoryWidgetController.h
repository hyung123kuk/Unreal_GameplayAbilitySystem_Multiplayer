// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKWidgetControllerBase.h"
#include "Item/Inventory.h"
#include "InventoryWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeInventoryValueSignature, UInventoryWidgetController*, InventoryController);

USTRUCT(BlueprintType)
struct FInventoryWidgetControllerParams
{
	GENERATED_BODY()

	FInventoryWidgetControllerParams() {}
	FInventoryWidgetControllerParams(const TMap<int, FUserItem>& consumableItems, const TArray<FUserItem>& characters) :
		ConsumableItems(consumableItems),
		Characters(characters) {}

	UPROPERTY(BlueprintReadOnly)
	TMap<int, FUserItem> ConsumableItems;

	UPROPERTY(BlueprintReadOnly)
	TArray<FUserItem> Characters;
};

UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API UInventoryWidgetController : public UHKWidgetControllerBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FInventoryWidgetControllerParams& Params);

	UPROPERTY(BlueprintAssignable, Category = "ChangeValue")
	FChangeInventoryValueSignature ChangeInventoryValueSignature;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TMap<int, FUserItem> ConsumableItems;

	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TArray<FUserItem> Characters;

};
