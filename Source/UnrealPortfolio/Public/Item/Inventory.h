// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemInfoData.h"
#include "Inventory.generated.h"


USTRUCT(BlueprintType)
struct FUserItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Id;

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
	void AddItemsToArray(const TArray<FUserItem> Items);
	void AddItemsToArray(const TArray<int> Ids, const TArray<int> Count);

	void AddItem(const int Id, const int Count);
	void AddItem(const FUserItem Item);
	
private:
	void AddConsumableItem(const FUserItem Item);
	void AddCharacter(const FUserItem Item);

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemInfoData> ConsumableItemsInfo;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UItemInfoData> CharacterItemsInfo;

	UPROPERTY()
	TMap<int ,FUserItem> ConsumableItems;

	UPROPERTY()
	TArray<FUserItem> Characters;
};
