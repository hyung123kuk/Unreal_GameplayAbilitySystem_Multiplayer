// Copyright Druid Mechanics


#include "Game/HKGameMode.h"
#include "Player/HKPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Game/HKGameInstance.h"
#include "Player/HKPlayerController.h"

void AHKGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
	UE_LOG(LogTemp, Log, TEXT("PreLogin"));
}

APlayerController* AHKGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* NewPlayerController = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);
	UE_LOG(LogTemp, Log, TEXT("Login"));

	FString Id = UGameplayStatics::ParseOption(Options, FString(TEXT("Id")));
	UHKGameInstance* GameInstance = Cast<UHKGameInstance>(GetGameInstance());

	FInGamePlayerInfo PlayerInfo;
	if (Id.IsEmpty())
	{
		PlayerInfo = GameInstance->GetAdminPlayerInfo();
		NewPlayerController->GetPlayerState<AHKPlayerState>()->SetPlayerName(PlayerInfo.UserID);
	}
	else
	{
		PlayerInfo = GameInstance->GetPlayerInfoWithID(Id);
		NewPlayerController->GetPlayerState<AHKPlayerState>()->SetPlayerName(Id);
	}

	Cast<AHKPlayerController>(NewPlayerController)->SettingUserInformation(PlayerInfo);

	return NewPlayerController;
}

void AHKGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Log, TEXT("PostLogin"));
}
