// Copyright Druid Mechanics


#include "Lobby/HKLobbyGameMode.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "Kismet/GameplayStatics.h"
#include "Lobby/HKLobbyPlayerState.h"
#include "Lobby/Room.h"
#include "HKDatabaseFunctionLibrary.h"
#include "HKBlueprintFunctionLibrary.h"
#include "Lobby/HKUILobbyPlayerController.h"
#include "Engine/Engine.h"
#include "Lobby/Store.h"
#include "MySQLConnection.h"


void AHKLobbyGameMode::StartPlay()
{
	Super::StartPlay();

	//TEST CODE
	if (GetNetMode() == NM_Standalone)
	{
		return;
	}

	InitializeStoreItemFromDatabase();
	NotifyOpenServerToLoginServer();
}

void AHKLobbyGameMode::NotifyOpenServerToLoginServer()
{
	FString localAddr = ToCStr(GetNetDriver()->GetLocalAddr()->ToString(true));
	UE_LOG(ServerLog, Warning, TEXT("GameMode RemoteAddress : %s"), *localAddr);
	bool bNotifyOpenServer = UHKDatabaseFunctionLibrary::NotifyOpenServer(UserData, localAddr, ServerName);

	// 데이터 베이스에 서버가 오픈됐음을 알리지 못함
	checkf(bNotifyOpenServer, TEXT("Failure to notify the database that the server is open"));
}

void AHKLobbyGameMode::NotifyUserCountToDataBase()
{
	UE_LOG(ServerLog, Warning, TEXT("NotifyUserCountToDataBase : %d"), AllPlayers.Num());
	FString localAddr = ToCStr(GetNetDriver()->GetLocalAddr()->ToString(true));
	UHKDatabaseFunctionLibrary::NotifyUserCount(UserData, localAddr, AllPlayers.Num());
}

void AHKLobbyGameMode::InitializeStoreItemFromDatabase()
{
	Store = GetWorld()->SpawnActorDeferred<AStore>(AStore::StaticClass(), FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	TArray<FStoreItemDefaultInfo> ItemsInfo;
	UHKDatabaseFunctionLibrary::GetStoreItemsInformation(UserData, ItemsInfo);
	Store->SetStoreItems(ItemsInfo);
	Store->FinishSpawning(FTransform::Identity);
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


	NotifyUserCountToDataBase();

	return NewPlayerController;
}

void AHKLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	//TEST CODE
	if (GetNetMode() == NM_Standalone)
		return;

	AHKLobbyPlayerState* NewPlayerState = NewPlayer->GetPlayerState<AHKLobbyPlayerState>();
	const FString Id = NewPlayerState->GetPlayerName();

	FString UserIntroduction;
	int UserGold = 0;
	int UserExp = 0;
	if (!UHKDatabaseFunctionLibrary::GetUserInformation(UserData, Id, UserIntroduction, UserGold, UserExp))
	{
		UE_LOG(ServerLog, Error, TEXT("유저(%s)정보를 데이터 베이스에서 찾기를 실패했습니다."), *Id);
		return;
	}
	UE_LOG(ServerLog, Warning, TEXT("입장한 유저(%s)의 정보 : 소개(%s) , 골드(%d) , 경험치(%d) "), *Id, *UserIntroduction, UserGold, UserExp);

	NewPlayerState->SetIntroduction(UserIntroduction);
	NewPlayerState->SetExp(UserExp);

	//PostLogin에서 Client에 PlayerController가 만들어짐이 확인 되어 _Implementation ToClient 실행 가능 
	AHKUILobbyPlayerController* LobbyPlayerController = Cast<AHKUILobbyPlayerController>(NewPlayer);
	
	FUserItemInfo UserItemInfo;
	UserItemInfo.PlayerGold = UserGold;
	TArray<int> ItemIds;
	TArray<int> ItemCounts;
	if (UHKDatabaseFunctionLibrary::GetUserItemsInformation(UserData, Id, ItemIds, ItemCounts))
	{
		UserItemInfo.ItemIds = ItemIds;
		UserItemInfo.ItemCounts = ItemCounts;
	}
	LobbyPlayerController->ItemInfo = UserItemInfo;

	FString PlayerIP = *NewPlayerState->GetNetConnection()->LowLevelGetRemoteAddress(true);
	UE_LOG(ServerLog, Warning, TEXT("입장한 유저(%s)의 IP 주소 : %s"), *Id, *PlayerIP);

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
	NotifyUserCountToDataBase();
	Super::Logout(Exiting);
}

bool AHKLobbyGameMode::TryToMakeAndEnterRoom(const APlayerController& Player, const FString& RoomName, const FString& RoomPassword, int MaxPlayers, FString& Message)
{
	const FString& PlayerId = GetPlayerIDWithController(Player);
	AHKLobbyPlayerState* PlayerState = FindPlayerState(PlayerId, Message);
	if (PlayerState == nullptr)
	{
		return false;
	}

	if (Rooms.Contains(RoomName))
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 이미 존재하는 방 이름(%s)을 만들려다 실패했습니다."), *PlayerId, *RoomName);
		Message = TEXT("이미 존재하는 방 이름 입니다.");
		return false;
	}

	if (RoomName.Len() > MaxRoomNameLen)
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 생성한 방 이름(%s)이 제한 글자수(%d) 보다 큽니다."), *PlayerId, *RoomName, MaxRoomNameLen);
		Message = FString(TEXT("방 이름은 %d 자리 이하만 가능합니다."), MaxRoomNameLen);
		return false;
	}

	if (RoomPassword.Len() > MaxRoomPasswordLen)
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 생성한 방 비밀번호(%s)가 제한 글자수(%d) 보다 큽니다."), *PlayerId, *RoomPassword, MaxRoomPasswordLen);
		Message = FString(TEXT("방 비밀번호는 %d 자리 이하만 가능합니다."), MaxRoomPasswordLen);
		return false;
	}

	ARoom* NewRoom = GetWorld()->SpawnActorDeferred<ARoom>(ARoom::StaticClass(), FTransform::Identity, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	NewRoom->InitRoomInfo(RoomName, RoomPassword, MaxPlayers);
	NewRoom->FinishSpawning(FTransform::Identity);
	Rooms.Add(RoomName, NewRoom);
	NewRoom->RoomDestroyDelegate.AddDynamic(this, &AHKLobbyGameMode::DestroyRoom);
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 방(%s)을 만들었습니다."), *PlayerId, *RoomName);

	if (TryToEnterRoom(Player, RoomName, RoomPassword, Message))
	{		
		NewRoom->ChangeRoomAdmin(PlayerState);
		return true;
	}

	return false;
}

bool AHKLobbyGameMode::TryToEnterRoom(const APlayerController& Player, const FString& RoomName, const FString& RoomPassword, FString& Message)
{
	const FString& PlayerId = GetPlayerIDWithController(Player);

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

bool AHKLobbyGameMode::TryToExitRoomAndGoToLobby(const APlayerController& Player, const FString& RoomName, FString& Message)
{
	const FString& PlayerId = GetPlayerIDWithController(Player);

	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 방(%s)에서 나가 로비로 가려고 시도합니다."), *PlayerId, *RoomName);
	ARoom* Room = FindRoom(RoomName, Message);
	if (Room == nullptr)
	{
		return false;
	}

	AHKLobbyPlayerState* PlayerState = FindPlayerState(PlayerId, Message);
	if (PlayerState == nullptr)
	{
		return false;
	}

	if (Room->ExitPlayer(PlayerState, Message))
	{
		LobbyPlayers.Add(PlayerState);
		UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 로비로 나가는 것을 성공합니다."), *PlayerId);
		return true;
	}

	UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 로비로 나가는 것을 실패합니다."), *PlayerId);
	return false;
}

bool AHKLobbyGameMode::TryToSendMessageOtherClients(const APlayerController& Player, const FString& ChattingMessage, FString& Message)
{
	const FString& PlayerId = GetPlayerIDWithController(Player);
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 채팅(%s)보내기를 시도합니다."), *PlayerId, *ChattingMessage);

	if (ChattingMessage.Len() > MaxChattingLen)
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 보낸 채팅(%s)길이가 제한 글자수(%d) 보다 큽니다."), *PlayerId, *ChattingMessage, MaxChattingLen);
		Message = FString(TEXT("채팅길이는 %d 자리 이하만 가능합니다."), MaxChattingLen);
		return false;
	}

	AHKLobbyPlayerState* PlayerState = FindPlayerState(PlayerId, Message);
	if (PlayerState == nullptr)
	{
		return false;
	}

	TArray<TObjectPtr<AHKLobbyPlayerState>> PlayersToReceive;
	ARoom* Room = PlayerState->GetEnteredRoom();
	FString RoomName;
	if (Room != nullptr)
	{
		RoomName = Room->GetRoomName();
		UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 방(%s)에서 채팅(%s)을 보냅니다.."), *PlayerId, *RoomName, *ChattingMessage);
		PlayersToReceive = Room->GetJoinPlayers();
	}
	else
	{
		UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 로비에서 채팅(%s)을 보냅니다.."), *PlayerId, *ChattingMessage);
		PlayersToReceive = LobbyPlayers;
	}

	for (AHKLobbyPlayerState* PlayerToReceive : PlayersToReceive)
	{
		AHKUILobbyPlayerController* LobbyPlayerController = Cast<AHKUILobbyPlayerController>(PlayerToReceive->GetPlayerController());
		if (LobbyPlayerController != nullptr)
		{
			LobbyPlayerController->NotifyReceiveChattingMessageToClient(PlayerId, ChattingMessage);
		}
	}

	if (!UHKDatabaseFunctionLibrary::RecordChatInDatabase(UserData, PlayerId, RoomName, ChattingMessage))
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 보낸 채팅(%s)을 데이터베이스에 넣는 것을 실패했습니다."), *PlayerId, *ChattingMessage);
	}

	return true;
}

bool AHKLobbyGameMode::TryToChangeIntroductionMessage(const APlayerController& Player, FString& Introduction, FString& Message)
{
	const FString& PlayerId = GetPlayerIDWithController(Player);
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) (%s)로 자기소개 변경을 시도합니다."), *PlayerId, *Introduction);
	AHKLobbyPlayerState* PlayerState = FindPlayerState(PlayerId, Message);
	if (PlayerState == nullptr)
	{
		return false;
	}
	
	if (!UHKDatabaseFunctionLibrary::ChangeUserIntroduction(UserData, PlayerId, Introduction))
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 보낸 소개(%s) 수정을 데이터베이스에 넣는 것을 실패했습니다."), *PlayerId, *Introduction);
		Message = TEXT("소개 변경이 실패하였습니다.");
		return false;
	}

	PlayerState->SetIntroduction(Introduction);
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) (%s)로 자기소개 변경을 성공합니다."), *PlayerId, *Introduction);
	return true;
}

bool AHKLobbyGameMode::TryToChangeReadyState(const APlayerController& Player, bool IsReady, FString& Message)
{
	const FString& PlayerId = GetPlayerIDWithController(Player);
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 준비상태를 (%s)로변경합니다."), *PlayerId, IsReady ? TEXT("해제 -> 준비"):TEXT("준비 -> 해제"));
	AHKLobbyPlayerState* PlayerState = FindPlayerState(PlayerId, Message);
	if (PlayerState == nullptr)
	{
		return false;
	}

	if (PlayerState->GetEnteredRoom() == nullptr)
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 들어간 방이 없어 준비상태 변경을 실패합니다."), *PlayerId);
		Message = TEXT("들어간 방이 없습니다.");
		return false;
	}

	PlayerState->SetIsReady(IsReady);
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 준비상태를 (%s)로변경에 성공합니다."), *PlayerId, IsReady ? TEXT("해제 -> 준비") : TEXT("준비 -> 해제"));
	return true;
}

bool AHKLobbyGameMode::TryToGameStart(const APlayerController& Player, const FString& RoomName, FString& Message)
{
	const FString& PlayerId = GetPlayerIDWithController(Player);
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 방(%s)에서 게임 시작을 시도합니다."), *PlayerId, *RoomName);
	AHKLobbyPlayerState* PlayerState = FindPlayerState(PlayerId, Message);
	if (PlayerState == nullptr)
	{
		return false;
	}

	ARoom* TryStartRoom = FindEnteredRoomWithPlayerState(PlayerState);
	if (TryStartRoom == nullptr)
	{
		return false;
	}

	if (!TryStartRoom->ReadyAllPlayers())
	{
		UE_LOG(ServerLog, Warning, TEXT("방(%s)에서 모든 플레이어가 준비가 되지 않아, 게임 시작에 실패합니다."), *PlayerId);
		Message = TEXT("모든 플레이어가 준비가 되지 않았습니다.");
		return false;
	}

	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 방(%s)에서 게임 시작에 성공합니다."), *PlayerId, *RoomName);
	GameStart(TryStartRoom);
	return true;
}

bool AHKLobbyGameMode::TryToFollowRoomUser(const APlayerController& Player, const FString& UserToFollow, const FString& RoomPassword, FString& Message)
{
	const FString& PlayerId = GetPlayerIDWithController(Player);
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 다른 플레이어(%s) 따라가기를 시도합니다."), *PlayerId, *UserToFollow);
	AHKLobbyPlayerState* FollowPlayerState = FindPlayerState(UserToFollow, Message);
	if (FollowPlayerState == nullptr)
	{
		return false;
	}

	if (FollowPlayerState->GetEnteredRoom() == nullptr)
	{
		UE_LOG(ServerLog, Error, TEXT("플레이어가(%s) 들어간 방이 없어 플레이어 따라가기를 실패합니다."), *UserToFollow);
		Message = TEXT("해당 플레이어는 들어간 방이 없습니다.");
		return false;
	}

	ARoom* Room = FindEnteredRoomWithPlayerState(FollowPlayerState);
	if (Room == nullptr)
	{
		return false;
	}
	if (TryToEnterRoom(Player, Room->GetRoomName(), RoomPassword, Message))
	{
		UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 다른 플레이어(%s) 따라가기에 성공합니다."), *PlayerId, *UserToFollow);
		return true;
	}

	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 다른 플레이어(%s) 따라가기에 실패합니다."), *PlayerId, *UserToFollow);
	return false;
}

bool AHKLobbyGameMode::TryToInviteLobbyUser(const APlayerController& Player, const FString& UserToInvite, FString& Message)
{
	const FString& PlayerId = GetPlayerIDWithController(Player);
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 다른 플레이어(%s) 초대를 시도합니다."), *PlayerId, *UserToInvite);

	AHKLobbyPlayerState* UserStateToInvite = FindPlayerState(UserToInvite, Message);
	if (UserStateToInvite == nullptr)
	{
		return false;
	}

	ARoom* InvitedUserRoom = FindEnteredRoomWithPlayerState(UserStateToInvite);
	if (InvitedUserRoom != nullptr)
	{
		UE_LOG(ServerLog, Warning, TEXT("초대받은 플레이어는(%s) 이미 다른 방에(%s) 있습니다."), *UserToInvite,*InvitedUserRoom->GetRoomName());
		Message = TEXT("해당 플레이어는 이미 다른 방에 입장했습니다.");
		return false;
	}   

	AHKLobbyPlayerState* FollowPlayerState = FindPlayerState(PlayerId, Message);
	if (FollowPlayerState == nullptr)
	{
		return false;
	}
	ARoom* SendRoom = FindEnteredRoomWithPlayerState(FollowPlayerState);
	if (SendRoom == nullptr)
	{
		UE_LOG(ServerLog, Warning, TEXT("초대한 플레이어는(%s) 들어간 방이 없습니다."), *UserToInvite);
		Message = TEXT("초대할 방이 없습니다.");
		return false;
	}

	AHKUILobbyPlayerController* InvitedPlayerController = Cast<AHKUILobbyPlayerController>(UserStateToInvite->GetPlayerController());
	InvitedPlayerController->NotifyInviteRoomMessageToClient(PlayerId, SendRoom->GetRoomName(), SendRoom->Password);
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 다른 플레이어(%s) 초대에 성공합니다."), *PlayerId, *UserToInvite);

	return true;
}

bool AHKLobbyGameMode::TryToPurchaseItem(APlayerController& Player, int ItemId, FString& Message)
{
	const FString& PlayerId = GetPlayerIDWithController(Player);
	UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 아이템(%d) 구매를 시도합니다."), *PlayerId, ItemId);
	AHKUILobbyPlayerController* LobbyPlayerCotroller = Cast<AHKUILobbyPlayerController>(&Player);
	int Cost;
	if (!Store->GetItemGoldWithItemID(ItemId, Cost))
	{
		UE_LOG(ServerLog, Warning, TEXT("아이템(%d)찾지 못해 플레이어가(%s) 아이템 구매에 실패합니다."), *PlayerId, ItemId);
		Message = TEXT("해당 아이템을 찾지 못했습니다.");
		return false;
	}

	int LeftGold = 0;

	if (!UHKDatabaseFunctionLibrary::PurchaseItem(UserData, PlayerId, ItemId, LeftGold))
	{
		UE_LOG(ServerLog, Warning, TEXT("플레이어가(%s) 아이템 구매에 실패합니다."), *PlayerId);
		Message = TEXT("아이템을 구매할 돈을 가지고 있지 않습니다.");
		return false;
	}

	FUserItemInfo NewUserItemInfo = LobbyPlayerCotroller->ItemInfo;
	NewUserItemInfo.PlayerGold = LeftGold;
	

	int itemIndex = -1;
	if (NewUserItemInfo.ItemIds.Find(ItemId, itemIndex))
	{
		NewUserItemInfo.ItemCounts[itemIndex]++;
	}
	else
	{
		NewUserItemInfo.ItemIds.Add(ItemId);
		NewUserItemInfo.ItemCounts.Add(1);
	}

	LobbyPlayerCotroller->ItemInfo = NewUserItemInfo;
	return true;
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
	if (DestroyRoom == nullptr)
	{
		return;
	}

	DestroyRoom->RoomDestroyDelegate.Clear();
	TArray<TObjectPtr<AHKLobbyPlayerState>> JoinPlayers = DestroyRoom->GetJoinPlayers();
	for (AHKLobbyPlayerState* JoinPlayer : JoinPlayers)
	{
		DestroyRoom->ExitPlayer(JoinPlayer, Message);
	}
	Rooms.Remove(RoomName);
	DestroyRoom->Destroy();
}

FString AHKLobbyGameMode::GetPlayerIDWithController(const APlayerController& PlayerController)
{
	FString PlayerID = PlayerController.GetPlayerState<AHKLobbyPlayerState>()->GetPlayerName();
	return PlayerID;
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




