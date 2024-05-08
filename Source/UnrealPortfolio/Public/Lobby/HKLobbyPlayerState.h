// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Lobby/HKLobbyGameMode.h"
#include "HKLobbyPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReadyStateChangedDelegate,FString,PlayerName, bool,NewReadyState);

class URoom;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKLobbyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	FOnReadyStateChangedDelegate OnReadyStateChanged;
	
public:
	/** Room */
	const URoom* GetRoom() const { return EnteredGameRoom; }
	void SetRoom(URoom* EnterGameRoom) { EnteredGameRoom = EnterGameRoom; }

	const bool GetIsRoomAdmin() const { return IsRoomAdmin; }
	void SetIsRoomAdmin(bool bIsRoomAdmin) { IsRoomAdmin = bIsRoomAdmin; }

	const bool GetIsReady() const { return IsReady; }
	UFUNCTION(Server, Reliable, BlueprintCallable)
	void SetIsReady_Server(bool bIsReady);

	/** Room End */

	/** Match Making */
	UFUNCTION(BlueprintCallable)
	const FString& GetListenServerIP() const { return ListenServerIP; }
	void SetListenServerIP(FString& ServerIP) { ListenServerIP = ServerIP; }
	/** Match Making End */


	UFUNCTION()
	void OnRep_ListenServerIP();

	void GameStart();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(Replicated)
	TObjectPtr<URoom> EnteredGameRoom;

	UPROPERTY(ReplicatedUsing = OnRep_ListenServerIP)
	FString ListenServerIP;
	
	UPROPERTY(Replicated)
	bool IsRoomAdmin;
	
	UPROPERTY(Replicated)
	bool IsReady;
};
