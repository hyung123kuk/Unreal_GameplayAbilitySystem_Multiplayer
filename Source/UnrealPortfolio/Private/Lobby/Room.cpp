// Copyright Druid Mechanics


#include "Lobby/Room.h"
#include "Lobby/HKLobbyPlayerState.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "Lobby/HKUILobbyPlayerController.h"

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

bool URoom::EnterPlayer(AHKLobbyPlayerState* NewPlayer,const FString& AttempPassword, FString& ErrorMessage) 
{
	if (!bPublicRoom && Password != AttempPassword)
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어(%s)의 방(%s) 입장 시도한 비밀번호가(%s) 일치하지 않습니다. 방 비밀번호 : %s"),*NewPlayer->GetName(),*Name,*AttempPassword,*Password);
		ErrorMessage = TEXT("비밀번호가 일치하지 않습니다.");
		return false;
	}

	JoinPlayers.Add(NewPlayer);
	NewPlayer->SetEnteredRoomName(Name);
	return true;
}

void URoom::ExitPlayer(AHKLobbyPlayerState* ExitPlayer)
{
	UE_LOG(ServerLog, Warning, TEXT("유저(%s)가 방(%s)을 나가려 합니다."),*ExitPlayer->GetPlayerName(), *Name);
	if (JoinPlayers.Contains(ExitPlayer))
	{
		UE_LOG(ServerLog, Error, TEXT("방(%s)에 해당유저(%s)가 없습니다.."), *Name, *ExitPlayer->GetPlayerName());
		return;
	}
	
	JoinPlayers.Remove(ExitPlayer);
	ExitPlayer->SetEnteredRoomName(TEXT(""));
	UE_LOG(ServerLog, Warning, TEXT("유저(%s)가 방(%s)을 나갔습니다."), *ExitPlayer->GetPlayerName(), *Name);

	if (JoinPlayers.Num() == 0)
	{
		UE_LOG(ServerLog, Warning, TEXT("방(%s)에 유저가 없어 방을 삭제합니다."), *Name);

		//TODO: 방 삭제
		return;
	}

	if (RoomAdminPlayer == ExitPlayer)
	{
		FString NewAdminPlayerName;
		UE_LOG(ServerLog, Warning, TEXT("방(%s)에 방장유저(%s)가 나가 새로운 유저(%s)가 방장이 됩니다."), *Name, *ExitPlayer->GetPlayerName(),*NewAdminPlayerName);

		//TODO: 방 권한 넘기기
		ChangeRoomAdmin(JoinPlayers[0]);
	}
}


int URoom::GetReadyPlayersCount() const
{
	int ReadyPlayerCount = 0;
	for (const AHKLobbyPlayerState* JoinPlayer : JoinPlayers)
	{
		if (JoinPlayer->GetIsReady())
		{
			ReadyPlayerCount++;
		}
	}

	return ReadyPlayerCount;
}



