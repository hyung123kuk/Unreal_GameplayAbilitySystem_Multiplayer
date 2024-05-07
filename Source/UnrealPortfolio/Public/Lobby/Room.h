// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Room.generated.h"

class AHKLobbyPlayerState;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API URoom : public UObject
{
	GENERATED_BODY()

public:

	URoom() {}

	void ChangeRoomAdmin(AHKLobbyPlayerState* NewRoomAdminPlayer);
	const AHKLobbyPlayerState* GetAdminPlayer() const { return RoomAdminPlayer; }
	const TArray<TObjectPtr<AHKLobbyPlayerState>> GetJoinPlayers() const { return JoinPlayers; }

	void EnterPlayer(AHKLobbyPlayerState* NewPlayer);

	bool ReadyAllPlayer() const;

	int GetAllPlayer() const;

	int GetReadyPlayer() const;

private:

	UPROPERTY()
	TObjectPtr<AHKLobbyPlayerState> RoomAdminPlayer;

	UPROPERTY()
	TArray<TObjectPtr<AHKLobbyPlayerState>> JoinPlayers;

};
