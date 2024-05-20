// Copyright Druid Mechanics


#include "Lobby/Room.h"
#include "Lobby/HKLobbyPlayerState.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/HKUILobbyPlayerController.h"
#include "UI/WidgetController/LobbyRoomInfoWidgetController.h"
#include "Lobby/HKUILobbyPlayerController.h"

ARoom::ARoom()
{
	PrimaryActorTick.bCanEverTick = false;
	bAlwaysRelevant = true;
	bReplicates = true;
	NetUpdateFrequency = 10.f;
}

void ARoom::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_DedicatedServer)
		return;
	
	SendChangedRoomInformationToClients();
}

void ARoom::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Name);
	DOREPLIFETIME(ThisClass, bPublicRoom);
	DOREPLIFETIME(ThisClass, MaxPlayer);
	DOREPLIFETIME(ThisClass, RoomAdminPlayer);
	DOREPLIFETIME(ThisClass, JoinPlayers);
}

void ARoom::InitRoomInfo(const FString& RoomName, const FString& RoomPassword, int RoomMaxPlayer)
{
	Name = RoomName;
	if (RoomPassword.Len() == 0)
		bPublicRoom = true;
	Password = RoomPassword;
	MaxPlayer = RoomMaxPlayer;
}

void ARoom::ChangeRoomAdmin(AHKLobbyPlayerState* NewRoomAdminPlayer)
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
		NewRoomAdminPlayer->SetIsReady(false);
	}
}

bool ARoom::EnterPlayer(AHKLobbyPlayerState* NewPlayer,const FString& AttempPassword, FString& ErrorMessage) 
{
	if (!bPublicRoom && Password != AttempPassword)
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어(%s)의 방(%s) 입장 시도한 비밀번호가(%s) 일치하지 않습니다. 방 비밀번호 : %s"),*NewPlayer->GetName(),*Name,*AttempPassword,*Password);
		ErrorMessage = TEXT("비밀번호가 일치하지 않습니다.");
		return false;
	}

	if (NewPlayer->GetEnteredRoom() == this)
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어(%s)의 방(%s)에 이미 입장하고 있으나 입장 시도를 했습니다. "), *NewPlayer->GetName(), *Name);
		ErrorMessage = TEXT("이미 방에 입장하였습니다.");
		return false;
	}

	if (!AreThereRemainingSeatInTheRoom())
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어(%s)의 방(%s)에 입장을 시도했으나 남은자리(%d/%d)가 없습니다. "), *NewPlayer->GetName(), *Name,JoinPlayers.Num(), MaxPlayer);
		ErrorMessage = TEXT("방에 남은 자리가 없습니다.");
		return false;
	}

	JoinPlayers.Add(NewPlayer);
	NewPlayer->SetEnteredRoom(this);
	return true;
}

bool ARoom::ExitPlayer(AHKLobbyPlayerState* ExitPlayer, FString& Message)
{
	UE_LOG(ServerLog, Warning, TEXT("유저(%s)가 방(%s)을 나가려 합니다."),*ExitPlayer->GetPlayerName(), *Name);
	
	if (!JoinPlayers.Contains(ExitPlayer))
	{
		UE_LOG(ServerLog, Error, TEXT("방(%s)에 해당유저(%s)가 없습니다.."), *Name, *ExitPlayer->GetPlayerName());
		Message = TEXT("방에 존재하지 않는 유저입니다.");
		return false;
	}

	JoinPlayers.Remove(ExitPlayer);
	ExitPlayer->SetIsReady(false);
	ExitPlayer->SetListenServerIP(TEXT(""));
	ExitPlayer->SetEnteredRoom(nullptr);
	UE_LOG(ServerLog, Warning, TEXT("유저(%s)가 방(%s)을 나갔습니다."), *ExitPlayer->GetPlayerName(), *Name);

	if (JoinPlayers.Num() == 0)
	{
		UE_LOG(ServerLog, Warning, TEXT("방(%s)에 유저가 없어 방을 삭제합니다."), *Name);
		NotifyRemoveRoomToClient();
		RoomDestroyDelegate.Broadcast(Name);
		ExitPlayer->SetIsRoomAdmin(false);
		return true;
	}

	if (RoomAdminPlayer == ExitPlayer)
	{
		FString NewAdminPlayerName;
		UE_LOG(ServerLog, Warning, TEXT("방(%s)에 방장유저(%s)가 나가 새로운 유저(%s)가 방장이 됩니다."), *Name, *ExitPlayer->GetPlayerName(),*NewAdminPlayerName);

		ChangeRoomAdmin(JoinPlayers[0]);
	}
	return true;
}

void ARoom::OnRep_JoinPlayers()
{
	SendChangedRoomInformationToClients();
}

void ARoom::OnRep_Name()
{
	SendChangedRoomInformationToClients();
}

void ARoom::OnRep_PublicRoom()
{
	SendChangedRoomInformationToClients();
}

void ARoom::OnRep_MaxPlayer()
{
	SendChangedRoomInformationToClients();
}

void ARoom::SendChangedRoomInformationToClients()
{
	AHKUILobbyPlayerController* LocalClientPlayerController = Cast<AHKUILobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	AHKLobbyPlayerState* LocalClientPlayerState = LocalClientPlayerController->GetPlayerState<AHKLobbyPlayerState>();

	if (!IsValid(LocalClientPlayerState) || !IsValid(LocalClientPlayerController))
	{
		return;
	}

	if (RoomInfoWidgetController == nullptr)
	{
		RoomInfoWidgetController = NewObject<ULobbyRoomInfoWidgetController>(this, ULobbyRoomInfoWidgetController::StaticClass());
	}
	LocalClientPlayerController->MakeLobbyRoomWidgetController(RoomInfoWidgetController);
	RoomInfoWidgetController->SetWidgetControllerParams(FLobbyRoomInfoWidgetControllerParams(Name, MaxPlayer, JoinPlayers.Num(),bPublicRoom));
}

bool ARoom::AreThereRemainingSeatInTheRoom()
{
	if (JoinPlayers.Num() >= MaxPlayer)
	{
		return false;
	}

	return true;
}

void ARoom::NotifyRemoveRoomToClient_Implementation()
{
	if (RoomInfoWidgetController)
	{
		AHKUILobbyPlayerController* LocalClientPlayerController = Cast<AHKUILobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
		LocalClientPlayerController->RemoveLobbyRoomWidgetController(RoomInfoWidgetController);
	}
}

int ARoom::GetReadyPlayersCount() const
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



