// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Store.generated.h"

class UTexture2D;
class UStoreWidgetController;

UENUM(BlueprintType)
enum class EStoreCategoryType : uint8
{
	Character,
	Consumable,
};

USTRUCT(BlueprintType)
struct FStoreItemDefaultInfo
{
	GENERATED_BODY()

	FStoreItemDefaultInfo() {}
	FStoreItemDefaultInfo(int id, const FString& name, int cost, const FString& textureName, const FString& explanation) :
		Id(id),
		Name(name),
		Cost(cost),
		TextureName(textureName),
		Explanation(explanation)
	{
		if (id >= 100)
		{
			ItemType = EStoreCategoryType::Consumable;
		}
		else
		{
			ItemType = EStoreCategoryType::Character;
		}
	}
	
	UPROPERTY(BlueprintReadOnly)
	int Id;
	
	UPROPERTY(BlueprintReadOnly)
	EStoreCategoryType ItemType;

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	int Cost;

	UPROPERTY(BlueprintReadOnly)
	FString TextureName;

	UPROPERTY(BlueprintReadOnly)
	FString Explanation;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> Texture;
};

UCLASS()
class UNREALPORTFOLIO_API AStore : public AActor
{
	GENERATED_BODY()
	
protected:	
	AStore();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void SetStoreItems(TArray<FStoreItemDefaultInfo> itemsInfo) { ItemsInfo = itemsInfo; }
	const TArray<FStoreItemDefaultInfo> GetStoreItems() { return ItemsInfo; }

	bool GetItemGoldWithItemID(int ItemID, int& Gold);

	void SendChangedStoreInformationToClients();

	UFUNCTION()
	void OnRep_ItemInfo();

private:
	UPROPERTY(ReplicatedUsing = OnRep_ItemInfo)
	TArray<FStoreItemDefaultInfo> ItemsInfo;

private:
	UPROPERTY()
	TObjectPtr<UStoreWidgetController> StoreInfoWidgetController;

};
