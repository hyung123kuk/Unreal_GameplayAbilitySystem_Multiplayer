// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Game/HKDedicatedServerGameModeBase.h"
#include "HKLobbyGameMode.generated.h"

class AHKLobbyPlayerState;
class URoom;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKLobbyGameMode : public AHKDedicatedServerGameModeBase
{
	GENERATED_BODY()
	
	
private:
	/** Login */
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;
	/** Login End */


	/** Match Making */
	UFUNCTION()
	void ChangePlayerReadyState(FString Id, bool NewReadyState);

	void PlayerAllReady(const URoom* Room);

	void GameStart(const URoom* Room);
	/** Match Making End */


private:
	UPROPERTY()
	TArray<URoom*> Rooms;

	UPROPERTY()
	TMap<FString, TObjectPtr<AHKLobbyPlayerState>> AllPlayers;

};
