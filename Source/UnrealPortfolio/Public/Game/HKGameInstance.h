// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Lobby/HKUILobbyPlayerController.h"
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
	void OnNetworkLevelChangeFailure(UWorld*, UNetDriver*, ENetworkFailure::Type FailType, const FString& ErrorMessage);
	bool GetNetworkLevelChangeErrorMessage(FString& ErrorMessage);
	// ~ Network Message

	// Store InGame Player Info Before Start Game
	void StoreInGamePlayerInfoBeforeGameStart(TArray<FInGamePlayerInfo> InGamePlayerInfoParams);
	const TArray<FInGamePlayerInfo>& GetPlayersInfo() const { return InGamePlayersInfo; } 
	FInGamePlayerInfo GetPlayerInfoWithID(FString ID);
	FInGamePlayerInfo GetAdminPlayerInfo();
	FInGamePlayerInfo GetMyGameInfo();
	// ~ Store InGame Player Info Before Start Game End

private:
	bool OccurredLevelChangeError;
	FString LevelChangeErrorMessage;
	
	//리슨서버에 들어가기전 임시로 저장해 놓는다.
	UPROPERTY()
	TArray<FInGamePlayerInfo> InGamePlayersInfo;

};
