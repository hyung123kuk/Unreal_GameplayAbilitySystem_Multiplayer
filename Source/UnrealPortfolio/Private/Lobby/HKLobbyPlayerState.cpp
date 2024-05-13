// Copyright Druid Mechanics


#include "Lobby/HKLobbyPlayerState.h"
#include "HKBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Lobby/HKUILobbyPlayerController.h"
#include "UI/WidgetController/RoomUserInfoWidgetControlle.h"


void AHKLobbyPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (GetNetMode() == NM_DedicatedServer)
	{
		return;
	}
	LocalClientPlayerState = Cast<AHKLobbyPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	LocalClientPlayerController = Cast<AHKUILobbyPlayerController>(UGameplayStatics::GetPlayerState(this, 0));
}

void AHKLobbyPlayerState::EnteredGameRoom()
{
	if (LocalClientPlayerState->GetEnteredRoomName() == EnteredGameRoomName)
	{
		bSameRoomAsLocalClient = true;
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
	if (LocalClientPlayerState->GetEnteredRoomName() == ExitedGameRoomName)
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

void AHKLobbyPlayerState::OnRep_GameRoomName()
{

	if (EnteredGameRoomName.IsEmpty() && !ExitedGameRoomName.IsEmpty())
	{
		ExitGameRoom();
	}

	if (!EnteredGameRoomName.IsEmpty())
	{
		EnteredGameRoom();
	}

	ExitedGameRoomName = EnteredGameRoomName;

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
	//방이 있고, 로컬 플레이어와 같은 방에 있다면
	if (!EnteredGameRoomName.IsEmpty() && bSameRoomAsLocalClient)
	{
		FString UserName = GetPlayerName();
		RoomInfoWidgetController->SetWidgetControllerParams(FRoomUserInfoWidgetControllerParams(UserName, IsRoomAdmin,IsReady, SelectCharacter));
	}
}

void AHKLobbyPlayerState::GameStart()
{
	if (IsRoomAdmin)
	{
		//방장 플레이어
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
	DOREPLIFETIME(ThisClass, EnteredGameRoomName);
}



