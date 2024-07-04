// Copyright Druid Mechanics


#include "Lobby/HKLobbyPlayerState.h"
#include "HKBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Lobby/HKUILobbyPlayerController.h"
#include "UI/WidgetController/RoomUserInfoWidgetControlle.h"
#include "UI/WidgetController/UserInfoWidgetController.h"
#include "Lobby/Room.h"

AHKLobbyPlayerState::AHKLobbyPlayerState()
{
	NetUpdateFrequency = 10.f;
}

void AHKLobbyPlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() != NM_DedicatedServer)
	{
		EnterLobbyNewPlayer();
	}
}

void AHKLobbyPlayerState::Destroyed()
{
	if (GetNetMode() != NM_DedicatedServer)
	{
		ExitGameRoom();
		ExitLobbyPlayer();
	}

	Super::Destroyed();
}

void AHKLobbyPlayerState::EnterLobbyNewPlayer()
{
	if (UserInfoWidgetController == nullptr)
	{
		UserInfoWidgetController = NewObject<UUserInfoWidgetController>(this, UserInfoWidgetControllerClass);
	}

	AHKUILobbyPlayerController* LocalClientPlayerController = Cast<AHKUILobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (IsValid(LocalClientPlayerController))
	{
		if (GetPlayerController() != nullptr)
		{
			//현재 플레이어
			LocalClientPlayerController->SetMyUserInfoWidgetController(UserInfoWidgetController);
		}
		else
		{
			//다른 플레이어
			LocalClientPlayerController->EnterLobbyUserWidgetController(UserInfoWidgetController);
		}

		SendChangePlayerInformationToLocalClient();
	}
}

void AHKLobbyPlayerState::ExitLobbyPlayer()
{
	AHKUILobbyPlayerController* LocalClientPlayerController = Cast<AHKUILobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (IsValid(LocalClientPlayerController))
	{
		LocalClientPlayerController->ExitLobbyUserWidgetController(UserInfoWidgetController);
	}
}

void AHKLobbyPlayerState::SendChangePlayerInformationToLocalClient()
{
	if (UserInfoWidgetController != nullptr)
	{
		FString UserName = GetPlayerName();
		UserInfoWidgetController->SetWidgetControllerParams(FUserInfoWidgetControllerParams(UserName, EnteredGameRoom, Introduction, PlayerLevel, PlayerExp));
	}
}

void AHKLobbyPlayerState::OnRep_Introduction()
{
	SendChangePlayerInformationToLocalClient();
}

void AHKLobbyPlayerState::OnRep_Level()
{
	SendChangePlayerInformationToLocalClient();
}

void AHKLobbyPlayerState::OnRep_Exp()
{
	SendChangePlayerInformationToLocalClient();
}

void AHKLobbyPlayerState::SetExistingUserWidgetControllers()
{
	//처음 들어왔을 경우 있던 값 초기화
	if (GetPlayerController() != nullptr)
	{
		AHKUILobbyPlayerController* LocalClientPlayerController = Cast<AHKUILobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

		if (IsValid(LocalClientPlayerController))
		{
			LocalClientPlayerController->CreateAndShowRoomWidget();
			TArray<AHKLobbyPlayerState*> RoomPlayers = EnteredGameRoom->GetJoinPlayers();
			for (AHKLobbyPlayerState* RoomPlayer : RoomPlayers)
			{
				if (RoomPlayer != this)
				{
					RoomPlayer->OnRep_GameRoom();
				}
			}
		}
	}
}

void AHKLobbyPlayerState::EnterGameRoom()
{
	AHKUILobbyPlayerController* LocalClientPlayerController = Cast<AHKUILobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	AHKLobbyPlayerState* LocalClientPlayerState = LocalClientPlayerController->GetPlayerState<AHKLobbyPlayerState>();

	if (!IsValid(LocalClientPlayerState) || !IsValid(LocalClientPlayerController))
	{
		return;
	}

	if (LocalClientPlayerState->GetEnteredRoom() == EnteredGameRoom)
	{
		bSameRoomAsLocalClient = true;
		SetExistingUserWidgetControllers();
		if (RoomInfoWidgetController == nullptr)
		{
			RoomInfoWidgetController = NewObject<URoomUserInfoWidgetControlle>(this, RoomInfoWidgetControllerClass);
		}
		LocalClientPlayerController->EnterSameRoomUserWidgetController(RoomInfoWidgetController);
	}
}

void AHKLobbyPlayerState::ExitGameRoom()
{
	bSameRoomAsLocalClient = false;
	AHKUILobbyPlayerController* LocalClientPlayerController = Cast<AHKUILobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	AHKLobbyPlayerState* LocalClientPlayerState = LocalClientPlayerController->GetPlayerState<AHKLobbyPlayerState>();

	if (!IsValid(LocalClientPlayerState) || !IsValid(LocalClientPlayerController))
	{
		return;
	}

	if (LocalClientPlayerState->GetEnteredRoom() == PrevGameRoom)
	{
		LocalClientPlayerController->ExitGameRoomUserWidgetController(RoomInfoWidgetController);
	}
}

void AHKLobbyPlayerState::OnRep_RoomAdmin()
{
	SendChangedRoomInformationToLocalClientInSameRoom();
}

void AHKLobbyPlayerState::OnRep_IsReady()
{
	SendChangedRoomInformationToLocalClientInSameRoom();
}

void AHKLobbyPlayerState::OnRep_SelectCharacter()
{
	SendChangedRoomInformationToLocalClientInSameRoom();
}

void AHKLobbyPlayerState::OnRep_GameRoom()
{
	if (EnteredGameRoom == nullptr && PrevGameRoom != nullptr)
	{
		ExitGameRoom();
	}

	if (EnteredGameRoom != nullptr)
	{
		EnterGameRoom();
	}

	PrevGameRoom = EnteredGameRoom;

	SendChangedRoomInformationToLocalClientInSameRoom();
	SendChangePlayerInformationToLocalClient();
}

void AHKLobbyPlayerState::SendChangedRoomInformationToLocalClientInSameRoom()
{
	//방이 있고, 로컬 플레이어와 같은 방에 있다면
	if (EnteredGameRoom != nullptr && bSameRoomAsLocalClient && RoomInfoWidgetController != nullptr)
	{
		FString UserName = GetPlayerName();
		RoomInfoWidgetController->SetWidgetControllerParams(FRoomUserInfoWidgetControllerParams(UserName, IsRoomAdmin,IsReady, SelectCharacter));
	}
}


void AHKLobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/** Lobby */
	DOREPLIFETIME(ThisClass, Introduction);
	DOREPLIFETIME(ThisClass, PlayerLevel);
	DOREPLIFETIME(ThisClass, PlayerExp);
	/** Lobby End*/

	/** Room */
	DOREPLIFETIME(ThisClass, IsRoomAdmin);
	DOREPLIFETIME(ThisClass, IsReady);
	DOREPLIFETIME(ThisClass, EnteredGameRoom);
	/** Room End*/
}



