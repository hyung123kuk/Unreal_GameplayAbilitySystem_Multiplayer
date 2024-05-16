// Copyright Druid Mechanics


#include "Lobby/HKLobbyGameMode.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/HKLobbyPlayerState.h"
#include "Lobby/Room.h"
#include "HKDatabaseFunctionLibrary.h"
#include "HKBlueprintFunctionLibrary.h"
#include "Engine/Engine.h"



void AHKLobbyGameMode::StartPlay()
{
	Super::StartPlay();

}

void AHKLobbyGameMode::Tick(float DeltaSeconds)
{
	//TEST CODE : Server State Test

}

bool AHKLobbyGameMode::AttemptedToLogin(const FString& Id, const FString& Password, FString& ErrorMessage)
{
	bool bAttempedToLogin = Super::AttemptedToLogin(Id, Password, ErrorMessage);
	UE_LOG(ServerLog, Warning, TEXT("이미 로그인된 아이디(%s)가 있는지 확인합니다."), *Id);

	if (AllPlayers.Contains(Id))
	{
		ErrorMessage = TEXT("이미 로그인된 아이디입니다.");
		UE_LOG(ServerLog, Error, TEXT("유저가 이미 로그인된 아이디(%s)로 로그인하여 실패했습니다."), *Id);
		bAttempedToLogin = false;
	}

	UE_LOG(ServerLog, Warning, TEXT("이미 로그인된 아이디(%s)가 없어 로비에 입장합니다."), *Id);

	return bAttempedToLogin;
}

void AHKLobbyGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	const FString Id = UGameplayStatics::ParseOption(Options, FString(TEXT("Id")));
	const FString Password = UGameplayStatics::ParseOption(Options, FString(TEXT("Password")));

	//TEST CODE
	return; 
	//TEST CODE

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

	//const FString Id = UGameplayStatics::ParseOption(Options, FString(TEXT("Id")));

	//TEST CODE
	const FString Id = FString::FromInt(ID);
	ID++;
	//TEST CODE

	AHKLobbyPlayerState* NewPlayerState = NewPlayerController->GetPlayerState<AHKLobbyPlayerState>();
	NewPlayerState->SetPlayerName(Id);

	AllPlayers.Add(Id, NewPlayerState);
	LobbyPlayers.Add(NewPlayerState);

	return NewPlayerController;
}

void AHKLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//TEST CODE
	if (GetNetMode() == NM_Standalone)
		return;

	AHKLobbyPlayerState* NewPlayerState = NewPlayer->GetPlayerState<AHKLobbyPlayerState>();
	FString PlayerIP = *NewPlayerState->GetNetConnection()->LowLevelGetRemoteAddress(true);

	UE_LOG(ServerLog, Warning, TEXT("입장한 유저(%s)의 IP 주소 : %s"), *NewPlayerState->GetPlayerName(), *PlayerIP);

}

void AHKLobbyGameMode::Logout(AController* Exiting)
{
	AHKLobbyPlayerState* ExitPlayerState = Exiting->GetPlayerState<AHKLobbyPlayerState>();
	if (!IsValid(ExitPlayerState))
	{
		Super::Logout(Exiting);
		return;
	}

	const FString PlayerID = ExitPlayerState->GetPlayerName();
	UE_LOG(ServerLog, Warning, TEXT("유저(%s)가 로그아웃을 시도합니다."), *PlayerID);
	FString Meesage;
	AHKLobbyPlayerState* PlayerState = FindPlayerState(PlayerID, Meesage);
	if (PlayerState == nullptr)
		UE_LOG(ServerLog, Warning, TEXT("로비에 유저(%s)가 존재하지 않습니다."), *PlayerID);

	ARoom* Room = FindEnteredRoomWithPlayerState(ExitPlayerState);
	if (Room)
	{
		UE_LOG(ServerLog, Warning, TEXT("유저(%s)가 들어간 방(%s)에서 나갑니다."), *PlayerID, *Room->GetName());
		Room->ExitPlayer(ExitPlayerState, Meesage);
	}
	AllPlayers.Remove(PlayerID);
	LobbyPlayers.Remove(ExitPlayerState);

	Super::Logout(Exiting);
}

bool AHKLobbyGameMode::TryToMakeAndEnterRoom(const FString& PlayerId, const FString& RoomName, const FString& RoomPassword, int MaxPlayers, FString& Message)
{
	if (Rooms.Contains(RoomName))
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 이미 존재하는 방 이름(%s)을 만들려다 실패했습니다."), *PlayerId, *RoomName);
		Message = TEXT("이미 존재하는 방 이름 입니다.");
		return false;
	}

	if (RoomName.Len() > MaxRoomNameLen)
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 생성한 방 이름(%s)이 제한 글자수(%d) 보다 큽니다."), *PlayerId, *RoomName, MaxRoomNameLen);
		Message = FString("방 이름은 %d 자리 이하만 가능합니다.", MaxRoomNameLen);
		return false;
	}

	if (RoomName.Len() > MaxRoomPasswordLen)
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 생성한 방 비밀번호(%s)가 제한 글자수(%d) 보다 큽니다."), *PlayerId, *RoomPassword, MaxRoomPasswordLen);
		Message = FString("방 비밀번호는 %d 자리 이하만 가능합니다.", MaxRoomPasswordLen);
		return false;
	}

	ARoom* NewRoom = GetWorld()->SpawnActorDeferred<ARoom>(ARoom::StaticClass(), FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	NewRoom->InitRoomInfo(RoomName, RoomPassword, MaxPlayers);
	NewRoom->FinishSpawning(FTransform::Identity);
	Rooms.Add(RoomName, NewRoom);
	NewRoom->RoomDestroyDelegate.AddDynamic(this, &AHKLobbyGameMode::DestroyRoom);
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 방(%s)을 만들었습니다."), *PlayerId, *RoomName);

	if (TryToEnterRoom(PlayerId, RoomName, RoomPassword, Message))
	{
		AHKLobbyPlayerState* PlayerState = FindPlayerState(PlayerId, Message);
		if (PlayerState == nullptr)
			return false;
		
		NewRoom->ChangeRoomAdmin(PlayerState);
		return true;
	}

	return false;
}

bool AHKLobbyGameMode::TryToEnterRoom(const FString& PlayerId, const FString& RoomName, const FString& RoomPassword, FString& Message)
{
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 방(%s)에 입장을 시도합니다."), *PlayerId, *RoomName);

	ARoom* Room = FindRoom(RoomName, Message);
	AHKLobbyPlayerState* PlayerState = FindPlayerState(PlayerId, Message);
	if (Room == nullptr || PlayerState == nullptr)
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 방(%s)에 입장을 실패합니다."), *PlayerId, *RoomName);
		return false;
	}

	if (PlayerState->GetEnteredRoom() != nullptr)
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 이미 방에 입장중입니다. 방(%s)에 입장을 실패합니다."), *PlayerId, *RoomName);
		Message = TEXT("이미 들어온 방이 있습니다.");
		return false;
	}

	if (Room->EnterPlayer(PlayerState, RoomPassword, Message))
	{
		UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 방(%s)에 입장했습니다."), *PlayerId, *RoomName);
		LobbyPlayers.Remove(PlayerState);
		return true;
	}

	UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 방(%s)에 입장을 실패합니다."), *PlayerId, *RoomName);
	return false;
}

bool AHKLobbyGameMode::TryToExitRoomAndGoToLobby(const FString& PlayerId, const FString& RoomName, FString& Message)
{
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 방(%s)에서 나가 로비로 가려고 시도합니다."), *PlayerId, *RoomName);
	ARoom* Room = FindRoom(RoomName, Message);
	AHKLobbyPlayerState* PlayerState = FindPlayerState(PlayerId, Message);
	if (Room->ExitPlayer(PlayerState, Message))
	{
		LobbyPlayers.Add(PlayerState);
		return true;
	}

	return false;
}

void AHKLobbyGameMode::ChangePlayerReadyState(FString Id, bool NewReadyState)
{
	if (AllPlayers.Contains(Id))
	{
		AHKLobbyPlayerState* PlayerState = AllPlayers.Find(Id)->Get();
		const ARoom* Room = FindEnteredRoomWithPlayerState(PlayerState);

		UE_LOG(ServerLog, Warning, TEXT("%d / %d"),Room->GetReadyPlayersCount(), Room->GetAllPlayersCount());

		if (Room->ReadyAllPlayers())
		{
			PlayerAllReady(Room);
		}
	}
}

void AHKLobbyGameMode::PlayerAllReady(const ARoom* Room)
{
	//TODO : 플레이어가 모두 준비한 후 방장이 시작하도록
	GameStart(Room);
}

void AHKLobbyGameMode::GameStart(const ARoom* Room)
{
	const AHKLobbyPlayerState* AdminPlayer = Room->GetAdminPlayer();
	const TArray<TObjectPtr<AHKLobbyPlayerState>> JoinPlayers = Room->GetJoinPlayers();
	FString AdminPlayerIP = *AdminPlayer->GetNetConnection()->LowLevelGetRemoteAddress(false);

	UE_LOG(ServerLog, Warning, TEXT("------------------------GameStart------------------------\n"));

	for (AHKLobbyPlayerState* Player : JoinPlayers)
	{
		Player->SetListenServerIP(AdminPlayerIP);
		UE_LOG(ServerLog, Log, TEXT("Player ID : %s"), *Player->GetPlayerName());
	}

	UE_LOG(ServerLog, Warning, TEXT("------------------------GameStart------------------------\n"));

}

void AHKLobbyGameMode::DestroyRoom(const FString& RoomName)
{
	FString Message;
	ARoom* DestroyRoom = FindRoom(RoomName,Message);
	DestroyRoom->RoomDestroyDelegate.Clear();
	TArray<TObjectPtr<AHKLobbyPlayerState>> JoinPlayers = DestroyRoom->GetJoinPlayers();
	for (AHKLobbyPlayerState* JoinPlayer : JoinPlayers)
	{
		DestroyRoom->ExitPlayer(JoinPlayer, Message);
	}
	Rooms.Remove(RoomName);
	DestroyRoom->Destroy();
}

ARoom* AHKLobbyGameMode::FindEnteredRoomWithPlayerState(AHKLobbyPlayerState* PlayerState)
{
	ARoom* EnteredRoom = PlayerState->GetEnteredRoom();
	return EnteredRoom;
}

ARoom* AHKLobbyGameMode::FindRoom(const FString& RoomName, FString& Message)
{
	if (Rooms.Contains(RoomName))
	{
		ARoom* Room = *Rooms.Find(RoomName);
		return Room;
	}

	Message = TEXT("방을 찾을 수 없습니다.");
	UE_LOG(ServerLog, Error, TEXT("방(%s)을 찾을 수 없습니다."), *RoomName);
	return nullptr;
}

AHKLobbyPlayerState* AHKLobbyGameMode::FindPlayerState(const FString& PlayerId, FString& Message)
{
	if (AllPlayers.Contains(PlayerId))
	{
		AHKLobbyPlayerState* Player = *AllPlayers.Find(PlayerId);
		return Player;
	}

	Message = TEXT("플레이어를 찾을 수 없습니다.");
	UE_LOG(ServerLog, Error, TEXT("플레이어(%s)를 찾을 수 없습니다."), *PlayerId);
	return nullptr;
}




