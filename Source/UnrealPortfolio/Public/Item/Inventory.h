// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item/ItemInfoData.h"
#include "Inventory.generated.h"

class UInventoryWidgetController;

USTRUCT(BlueprintType)
struct FUserItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Id;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int UniqueId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Count;

	UPROPERTY()
	FItemInfomation ItemInfo;

};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API UInventory : public UObject
{
	GENERATED_BODY()
	
public:
	void ReSettingItems(const TArray<int> Ids, const TArray<int> Count);

	void AddItemsToArray(const TArray<FUserItem> Items);
	void AddItemsToArray(const TArray<int> Ids, const TArray<int> Count);

	void AddItem(const int Id, const int Count, int UniqueID = 0);
	void AddItem(const FUserItem Item);

public:
	void AddEquipmentItem(const FUserItem Item); // InGame

private:
	void AddConsumableItem(const FUserItem Item);
	void AddCharacter(const FUserItem Item);

	void SendChangedInventoryInformationToClients();

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemInfoData> ConsumableItemsInfo;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemInfoData> EquipmentItemsInfo;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemInfoData> CharacterItemsInfo;

	UPROPERTY()
	TMap<int ,FUserItem> ConsumableItems;
	
	UPROPERTY()
	TMap<int, FUserItem> EquipmentItems;

	UPROPERTY()
	TArray<FUserItem> Characters;

private:
	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

};
