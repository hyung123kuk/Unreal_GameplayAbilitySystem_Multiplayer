// Copyright Druid Mechanics


#include "Lobby/HKLobbyPlayerState.h"
#include "HKBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Lobby/HKUILobbyPlayerController.h"
#include "UI/WidgetController/RoomUserInfoWidgetControlle.h"
#include "Lobby/Room.h"

AHKLobbyPlayerState::AHKLobbyPlayerState()
{
	NetUpdateFrequency = 10.f;
}

void AHKLobbyPlayerState::SetExistingUserWidgetControllers()
{
	//��� �� �� �ڽ��� ��� ���� �ִ� ���� �����´�.
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
	AHKLobbyPlayerState* LocalClientPlayerState = Cast<AHKLobbyPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	AHKUILobbyPlayerController* LocalClientPlayerController = Cast<AHKUILobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

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
	AHKLobbyPlayerState* LocalClientPlayerState = Cast<AHKLobbyPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	AHKUILobbyPlayerController* LocalClientPlayerController = Cast<AHKUILobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

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
			//World->ServerTravel("/Game/Maps/Game?listen");
		}
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Loading"), true, FString::Printf(TEXT("NextLevel=Game?Host=true")));	
	}
	else
	{
		APlayerController* PlayerController = GetPlayerController();
		if (PlayerController)
		{
			//PlayerController->ClientTravel(ListenServerIP, ETravelType::TRAVEL_Absolute);
		}
		UGameplayStatics::OpenLevel(GetWorld(), TEXT("Loading"), true, FString::Printf(TEXT("NextLevel=192.168.0.101")));//, *ListenServerIP,*GetPlayerName()));
	}
}

void AHKLobbyPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ListenServerIP);
	DOREPLIFETIME(ThisClass, IsRoomAdmin);
	DOREPLIFETIME(ThisClass, IsReady);
	DOREPLIFETIME(ThisClass, EnteredGameRoom);
}



