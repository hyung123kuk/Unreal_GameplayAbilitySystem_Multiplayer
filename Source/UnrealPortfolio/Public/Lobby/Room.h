// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Room.generated.h"

class AHKLobbyPlayerState;

UCLASS()
class UNREALPORTFOLIO_API URoom : public UObject
{
	GENERATED_BODY()

public:
	URoom() {}
	void RoomInfoSettings(const FString& RoomName, const FString& RoomPassword, int RoomMaxPlayer)
	{
		Name = RoomName;
		if (RoomPassword.Len() == 0)
			bPublicRoom = true;
		Password = RoomPassword;
		MaxPlayer = RoomMaxPlayer;
	}

	/** Change room information */
	void ChangeRoomAdmin(AHKLobbyPlayerState* NewRoomAdminPlayer);
	bool EnterPlayer(AHKLobbyPlayerState* NewPlayer, const FString& AttempPassword, FString& Message);
	void ExitPlayer(AHKLobbyPlayerState* ExitPlayer);
	/** End Change room information */
	
public:
	const AHKLobbyPlayerState* GetAdminPlayer() const { return RoomAdminPlayer; }
	const TArray<TObjectPtr<AHKLobbyPlayerState>> GetJoinPlayers() const { return JoinPlayers; }
	
	int GetReadyPlayersCount() const;
	int GetAllPlayersCount() const { return JoinPlayers.Num(); }
	bool ReadyAllPlayers() const { return GetAllPlayersCount() == GetReadyPlayersCount(); }

private:
	UPROPERTY()
	TObjectPtr<AHKLobbyPlayerState> RoomAdminPlayer;

	UPROPERTY()
	TArray<TObjectPtr<AHKLobbyPlayerState>> JoinPlayers;

	FString Name;
	FString Password;
	bool bPublicRoom;
	int MaxPlayer;
};
