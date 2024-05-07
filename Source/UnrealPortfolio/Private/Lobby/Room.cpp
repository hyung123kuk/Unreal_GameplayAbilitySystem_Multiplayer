// Copyright Druid Mechanics


#include "Lobby/Room.h"
#include "Lobby/HKLobbyPlayerState.h"

void URoom::ChangeRoomAdmin(AHKLobbyPlayerState* NewRoomAdminPlayer)
{
	if (JoinPlayers.Contains(NewRoomAdminPlayer))
	{
		//기존 룸매니저
		AHKLobbyPlayerState* PrevRoomAdmin = RoomAdminPlayer;
		if (PrevRoomAdmin)
			PrevRoomAdmin->SetIsRoomAdmin(false);

		//룸매니저 변경
		RoomAdminPlayer = NewRoomAdminPlayer;
		NewRoomAdminPlayer->SetIsRoomAdmin(true);
	}
}

void URoom::EnterPlayer(AHKLobbyPlayerState* NewPlayer)
{
	JoinPlayers.Add(NewPlayer);
}

bool URoom::ReadyAllPlayer() const
{
	return GetAllPlayer() == GetReadyPlayer();
}

int URoom::GetAllPlayer() const
{
	return JoinPlayers.Num();
}

int URoom::GetReadyPlayer() const
{
	int ReadyPlayerCount = 0;
	for (const AHKLobbyPlayerState* Player : JoinPlayers)
	{
		if (Player->GetIsReady())
		{
			ReadyPlayerCount++;
		}
	}

	return ReadyPlayerCount;
}

