// Copyright Druid Mechanics


#include "Lobby/HKLobbyPlayerState.h"
#include "HKBlueprintFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void AHKLobbyPlayerState::OnRep_ListenServerIP()
{
	if (!IsValid(GetPlayerController()))
	{
		return;
	}

	GameStart();
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
	DOREPLIFETIME(ThisClass, EnteredGameRoom);
}

void AHKLobbyPlayerState::SetIsReady_Server_Implementation(bool bIsReady)
{

	UE_LOG(LogTemp,Log,TEXT("prev : %s , new : %s"),IsReady?TEXT("true"):TEXT("false"), bIsReady ? TEXT("true") : TEXT("false"))

	if (IsReady == bIsReady)
	{
		return;
	}

	IsReady = bIsReady;
	OnReadyStateChanged.Broadcast(GetPlayerName(), IsReady);
}


