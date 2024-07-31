// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item/ItemInfoData.h"
#include "UI/Widget/HKSlotWidget.h"
#include "Inventory.generated.h"

class UInventoryWidgetController;

USTRUCT(BlueprintType)
struct FUserItem : public FSlotStruct
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FItemInfomation ItemInfo;

};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FItemDelegate, FUserItem, ItemInfo);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API UInventory : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FItemDelegate NewItemDelegate;
	UPROPERTY(BlueprintAssignable)
	FItemDelegate ChangeItemValueDelegate;
	UPROPERTY(BlueprintAssignable)
	FItemDelegate RemoveItemValueDelegate;

	UPROPERTY(BlueprintAssignable)
	FItemDelegate UseItemValueDelegate;

public:
	void ReSettingItems(const TArray<int> Ids, const TArray<int> Count);

	void AddItemsToArray(const TArray<FUserItem> Items);
	void AddItemsToArray(const TArray<int> Ids, const TArray<int> Count);

	void AddItem(const int Id, const int Count, int UniqueID = 0);
	void AddItem(const FUserItem Item);

	void UseItem(const int Id, const int UniqueID = 0); //Only Client

	UFUNCTION(BlueprintCallable)
	void RemoveItem(const int Id, int count = 1, const int UniqueID = 0);

	UFUNCTION(BlueprintCallable)
	FUserItem FindItem(const int Id, const int UniqueId);

private:
	void AddEquipmentItem(const FUserItem Item); 
	void UseEquipmentItem(const int Id, const int UniqueId);
	void RemoveEquipmentItem(const int Id, const int UniqueId);

	void AddConsumableItem(const FUserItem Item);
	void UseConsumableItem(const int Id);
	void RemoveConsumableItem(const int Id, int count = 1);

private:
	void AddCharacter(const FUserItem Item);
	void RemoveCharacter(const int Id);

	void SendChangedInventoryInformationToClients();

	FUserItem MakeNewUserItem(const FUserItem UserInfo, const  FItemInfomation ItemInfo);

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
