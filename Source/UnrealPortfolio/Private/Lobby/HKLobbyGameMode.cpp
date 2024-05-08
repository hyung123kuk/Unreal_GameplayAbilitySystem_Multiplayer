// Copyright Druid Mechanics


#include "Lobby/HKLobbyGameMode.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/HKLobbyPlayerState.h"
#include "Lobby/Room.h"
#include "HKDatabaseFunctionLibrary.h"


bool AHKLobbyGameMode::AttemptedToLogin(const FString& Id, const FString& Password, FString& ErrorMessage)
{
	bool bAttempedToLogin = Super::AttemptedToLogin(Id, Password, ErrorMessage);
	
	if (AllPlayers.Contains(Id))
	{
		ErrorMessage = TEXT("이미 로그인된 아이디입니다.");
		UE_LOG(ServerLog, Error, TEXT("This ID is already logged in  < ID : %s Password : %s > "), *Id, *Password);
		bAttempedToLogin = false;
	}

	return bAttempedToLogin;
}

void AHKLobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	const FString Id = UGameplayStatics::ParseOption(Options, FString(TEXT("Id")));
	const FString Password = UGameplayStatics::ParseOption(Options, FString(TEXT("Password")));
	if (!AttemptedToLogin(Id, Password, ErrorMessage))
	{
		return;
	}
}

APlayerController* AHKLobbyGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	//TEST CODE
	if (GetNetMode() == NM_Standalone)
	{
		return NewPlayerController;
	}

	const FString Id = UGameplayStatics::ParseOption(Options, FString(TEXT("Id")));
	AHKLobbyPlayerState* NewPlayerState = NewPlayerController->GetPlayerState<AHKLobbyPlayerState>();
	NewPlayerState->SetPlayerName(Id);

	AllPlayers.Add(Id, NewPlayerState);

	return NewPlayerController;
}

void AHKLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//TEST CODE
	if (GetNetMode() == NM_Standalone)
		return;

	AHKLobbyPlayerState* NewPlayerState = NewPlayer->GetPlayerState<AHKLobbyPlayerState>();
	FString PlayerIP = *NewPlayerState->GetNetConnection()->LowLevelGetRemoteAddress(false);

	UE_LOG(ServerLog, Warning, TEXT("Player IP : %s"), *PlayerIP);

	//TEST CODE

	if (Rooms.Num() == 0)
	{
		URoom* NewRoom = NewObject<URoom>();
		NewRoom->EnterPlayer(NewPlayerState);
		NewRoom->ChangeRoomAdmin(NewPlayerState);
		
		Rooms.Add(NewRoom);
	}
	else
	{
		Rooms[0]->EnterPlayer(NewPlayerState);
	}
	
	NewPlayerState->SetRoom(Rooms[0]);
	NewPlayerState->OnReadyStateChanged.AddDynamic(this, &AHKLobbyGameMode::ChangePlayerReadyState);


	//TEST CODE END

}

void AHKLobbyGameMode::Logout(AController* Exiting)
{
	const AHKLobbyPlayerState* NewPlayerState = Exiting->GetPlayerState<AHKLobbyPlayerState>();
	if (::IsValid(NewPlayerState))
	{
		const FString PlayerID = NewPlayerState->GetName();
		if (AllPlayers.Contains(PlayerID))
		{
			AllPlayers.Remove(PlayerID);
		}
	}
}

void AHKLobbyGameMode::ChangePlayerReadyState(FString Id, bool NewReadyState)
{
	if (AllPlayers.Contains(Id))
	{
		const AHKLobbyPlayerState* PlayerState = AllPlayers.Find(Id)->Get();
		const URoom* Room = PlayerState->GetRoom();

		UE_LOG(ServerLog, Warning, TEXT("%d / %d"),Room->GetReadyPlayer(), Room->GetAllPlayer());

		if (Room->ReadyAllPlayer())
		{
			PlayerAllReady(Room);
		}
	}
}

void AHKLobbyGameMode::PlayerAllReady(const URoom* Room)
{
	//TODO : 플레이어가 모두 준비한 후 방장이 시작하도록
	GameStart(Room);
}

void AHKLobbyGameMode::GameStart(const URoom* Room)
{
	const AHKLobbyPlayerState* AdminPlayer = Room->GetAdminPlayer();
	const TArray<TObjectPtr<AHKLobbyPlayerState>> JoinPlayers = Room->GetJoinPlayers();
	FString AdminPlayerIP = *AdminPlayer->GetNetConnection()->LowLevelGetRemoteAddress(false);

	UE_LOG(ServerLog, Warning, TEXT("------------------------GameStart------------------------\n"));

	for (AHKLobbyPlayerState* Player : JoinPlayers)
	{
		Player->SetListenServerIP(AdminPlayerIP);
		UE_LOG(ServerLog, Log, TEXT("Player ID : %s"), *Player->GetName());
	}

	UE_LOG(ServerLog, Warning, TEXT("------------------------GameStart------------------------\n"));

}




