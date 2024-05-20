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
			//���� �÷��̾�
			LocalClientPlayerController->SetMyUserInfoWidgetController(UserInfoWidgetController);
		}
		else
		{
			//�ٸ� �÷��̾�
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
		FString EnterRoomName = EnteredGameRoom != nullptr ? EnteredGameRoom->GetRoomName() : TEXT("");
		UserInfoWidgetController->SetWidgetControllerParams(FUserInfoWidgetControllerParams(UserName, EnterRoomName, Introduction, PlayerLevel, PlayerExp));
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

void AHKLobbyPlayerState::OnRep_Gold()
{
}

void AHKLobbyPlayerState::SetExistingUserWidgetControllers()
{
	//ó�� ������ ��� �ִ� �� �ʱ�ȭ
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

void AHKLobbyPlayerState::OnRep_ListenServerIP()
{
	if (!IsValid(GetPlayerController()))
	{
		return;
	}

	GameStart();
}

void AHKLobbyPlayerState::SendChangedRoomInformationToLocalClientInSameRoom()
{
	//���� �ְ�, ���� �÷��̾�� ���� �濡 �ִٸ�
	if (EnteredGameRoom != nullptr && bSameRoomAsLocalClient && RoomInfoWidgetController != nullptr)
	{
		FString UserName = GetPlayerName();
		RoomInfoWidgetController->SetWidgetControllerParams(FRoomUserInfoWidgetControllerParams(UserName, IsRoomAdmin,IsReady, SelectCharacter));
	}
}

void AHKLobbyPlayerState::GameStart()
{
	if (IsRoomAdmin)
	{
		//���� �÷��̾�
		UWorld* World = GetWorld();
		if (World)
		{
			UGameplayStatics::OpenLevel(GetWorld(), FName(*FString("Game")), true, "listen");
		}
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Loading"), true, FString::Printf(TEXT("NextLevel=Game?Host=true")));	
	}
	else
	{
		APlayerController* PlayerController = GetPlayerController();
		if (PlayerController)
		{
			PlayerController->ClientTravel("127.0.0.1:7777", ETravelType::TRAVEL_Absolute);
		}
		//UGameplayStatics::OpenLevel(GetWorld(), TEXT("Loading"), true, FString::Printf(TEXT("NextLevel=192.168.0.101")));//, *ListenServerIP,*GetPlayerName()));
	}
}

void AHKLobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/** Lobby */
	DOREPLIFETIME(ThisClass, Introduction);
	DOREPLIFETIME(ThisClass, PlayerLevel);
	DOREPLIFETIME(ThisClass, PlayerExp);
	DOREPLIFETIME(ThisClass, PlayerGold);
	/** Lobby End*/

	/** Room */
	DOREPLIFETIME(ThisClass, ListenServerIP);
	DOREPLIFETIME(ThisClass, IsRoomAdmin);
	DOREPLIFETIME(ThisClass, IsReady);
	DOREPLIFETIME(ThisClass, EnteredGameRoom);
	/** Room End*/
}



