// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Store.generated.h"

class UTexture2D;

USTRUCT()
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
	{}
	
	UPROPERTY()
	int Id;
	
	UPROPERTY()
	FString Name;

	UPROPERTY()
	int Cost;

	UPROPERTY()
	FString TextureName;

	UPROPERTY()
	FString Explanation;

	UPROPERTY()
	TObjectPtr<UTexture2D> Texture;
};

UCLASS()
class UNREALPORTFOLIO_API AStore : public AActor
{
	GENERATED_BODY()
	
protected:	
	AStore();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	void SetStoreItems(TArray<FStoreItemDefaultInfo> itemsInfo) { ItemsInfo = itemsInfo; }
	const TArray<FStoreItemDefaultInfo> GetStoreItems() { return ItemsInfo; }

	void InitializeSaleItems(TArray<FStoreItemDefaultInfo> Items);

	UFUNCTION()
	void OnRep_ItemInfo();

private:
	UPROPERTY(ReplicatedUsing = OnRep_ItemInfo)
	TArray<FStoreItemDefaultInfo> ItemsInfo;

};
