// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "ItemInfoData.generated.h"

class UGameplayAbility;
class UGameplayEffect;

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
	int Id = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Explanation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemCategoryType ItemType;

	//Character에서 저장용
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EquipmentTag;

	//몽타주에 사용할 추가 태그
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag MontageTypeTag;
	
	//사용시 발생하는 태그 (ex.몽타주)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayTag> TriggerTags;

	//사용시 부여되는 어빌리티 ( 무기 교체시 해당 무기의 공격 어빌리티를 가진다던가.. )
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> GiveAbilities;

	//사용시 발생하는 Effec ( 물약을 먹으면 회복을 한다던가..)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> OccurEffects;

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
