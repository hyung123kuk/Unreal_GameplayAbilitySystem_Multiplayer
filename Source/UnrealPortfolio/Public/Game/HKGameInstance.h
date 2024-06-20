// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HKGameInstance.generated.h"

class UInventory;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	//Netwrok Message
	void OnNetworkLevelChangeFailure(UWorld*, UNetDriver*, ENetworkFailure::Type FailType, const FString& ErrorMessage);
	bool GetNetworkLevelChangeErrorMessage(FString& ErrorMessage);
	// ~ Network Message

private:
	bool OccurredLevelChangeError;
	FString LevelChangeErrorMessage;

	//서버가 변경되도 인벤토리가 유지되어야 하므로 Instance에 놓는다.
	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInventory> InventoryClass;

	UPROPERTY()
	TObjectPtr<UInventory> Inventory;
};
