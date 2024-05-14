// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Game/HKDedicatedServerGameModeBase.h"
#include "HKLobbyGameMode.generated.h"

class AHKLobbyPlayerState;
class ARoom;


/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKLobbyGameMode : public AHKDedicatedServerGameModeBase
{
	GENERATED_BODY()
	
	virtual void StartPlay() override;
	
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
	bool TryToMakeAndEnterRoom(const FString& PlayerId, const FString& RoomName, const FString& RoomPassword, int MaxPlayers, FString& Message);
	bool TryToEnterRoom(const FString& PlayerId, const FString& RoomName, const FString& RoomPassword, FString& Message);
	bool TryToExitRoomAndGoToLobby(const FString& PlayerId, const FString& RoomName, FString& Message);
	/** Client's request End */

private:
	/** Match Making */
	UFUNCTION()
	void ChangePlayerReadyState(FString Id, bool NewReadyState);
	void PlayerAllReady(const ARoom* Room);
	void GameStart(const ARoom* Room);
	/** Match Making End */

private:
	/** Room */
	UFUNCTION()
	void DestroyRoom(const FString& RoomName);
	/** Room End */

private:
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

