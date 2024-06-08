// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemInfoData.generated.h"

UENUM(BlueprintType)
enum class EItemCategoryType : uint8
{
	Consumable,
	Equipment,
	Character,
	None,
};

USTRUCT(BlueprintType)
struct FItemInfomation
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Id;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Explanation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemCategoryType ItemType;
};

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UItemInfoData : public UDataAsset
{
	GENERATED_BODY()

public:
	FItemInfomation FindItemInfoForItemID(const int Id) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FItemInfomation> AttributeInformation;

};
