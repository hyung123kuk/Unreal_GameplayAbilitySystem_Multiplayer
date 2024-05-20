// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Game/HKDedicatedServerGameModeBase.h"
#include "HKLobbyGameMode.generated.h"

class AHKLobbyPlayerState;
class AHKUILobbyPlayerController;
class ARoom;


/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKLobbyGameMode : public AHKDedicatedServerGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	/** Login */
	virtual bool AttemptedToLogin(const FString& ID, const FString& Password, FString& ErrorMessage) override;
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	/** Login End */

public:
	/** Client's request */
	bool TryToMakeAndEnterRoom(const APlayerController& Player, const FString& RoomName, const FString& RoomPassword, int MaxPlayers, FString& Message);
	bool TryToEnterRoom(const APlayerController& Player, const FString& RoomName, const FString& RoomPassword, FString& Message);
	bool TryToExitRoomAndGoToLobby(const APlayerController& Player, const FString& RoomName, FString& Message);
	bool TryToSendMessageOtherClients(const APlayerController& Player, const FString& ChattingMessage, FString& Message);
	bool TryToChangeIntroductionMessage(const APlayerController& Player, FString& Introduction, FString& Message);
	bool TryToChangeReadyState(const APlayerController& Player, bool IsReady, FString& Message);
	bool TryToGameStart(const APlayerController& Player,const FString& RoomName, FString& Message);
	bool TryToFollowRoomUser(const APlayerController& Player,const FString& UserToFollow, const FString& RoomPassword, FString& Message);
	/** Client's request End */

private:
	/** Match Making */
	void GameStart(const ARoom* Room);
	/** Match Making End */

private:
	/** Room */
	UFUNCTION()
	void DestroyRoom(const FString& RoomName);
	/** Room End */

private:
	FString GetPlayerIDWithController(const APlayerController& PlayerController);
	ARoom* FindEnteredRoomWithPlayerState(AHKLobbyPlayerState* PlayerState);
	ARoom* FindRoom(const FString& RoomName, FString& Message);
	AHKLobbyPlayerState* FindPlayerState(const FString& PlayerId, FString& Message);

	
private:
	UPROPERTY()
	TMap<FString, TObjectPtr<ARoom>> Rooms;

	UPROPERTY()
	TMap<FString, TObjectPtr<AHKLobbyPlayerState>> AllPlayers;

	UPROPERTY()
	TArray<TObjectPtr<AHKLobbyPlayerState>> LobbyPlayers;

	int ID;
};

