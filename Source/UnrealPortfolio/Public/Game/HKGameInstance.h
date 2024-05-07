// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HKGameInstance.generated.h"


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
	void OnNetworkLevelChangeFailure(UWorld*, ETravelFailure::Type, const FString& ErrorMessage);
	bool GetNetworkLevelChangeErrorMessage(FString& ErrorMessage);
	// ~ Network Message

private:
	bool OccurredLevelChangeError;
	FString LevelChangeErrorMessage;

};
