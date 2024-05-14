// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Lobby/HKLobbyGameMode.h"
#include "HKLobbyPlayerState.generated.h"

class URoomUserInfoWidgetControlle;
class AHKUILobbyPlayerController;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKLobbyPlayerState : public APlayerState
{
	GENERATED_BODY()
	

public:
	AHKLobbyPlayerState();

	/** Only Server Func */
	void SetEnteredRoom(ARoom* EnterGameRoom) { EnteredGameRoom = EnterGameRoom; }
	void SetIsRoomAdmin(bool bIsRoomAdmin) { IsRoomAdmin = bIsRoomAdmin; }
	void SetListenServerIP(FString ServerIP) { ListenServerIP = ServerIP; }
	void SetIsReady(bool bIsReady) { IsReady = bIsReady; }
	/** Only Server Func End*/
	
	/** Room Info Get*/
	UFUNCTION(BlueprintCallable)
	ARoom* GetEnteredRoom() const { return EnteredGameRoom; }
	const bool GetIsRoomAdmin() const { return IsRoomAdmin; }
	const bool GetIsReady() const { return IsReady; }
	/** Room Info Get End*/

	/** Match Making */
	UFUNCTION(BlueprintCallable)
	const FString& GetListenServerIP() const { return ListenServerIP; }
	/** Match Making End */

	void GameStart();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	/** Room */
	void SetExistingUserWidgetControllers();
	void EnterGameRoom();
	void ExitGameRoom();
	void SendChangedRoomInformationToLocalClientInSameRoom();

	UFUNCTION()
	void OnRep_GameRoom();
	UFUNCTION()
	void OnRep_RoomAdmin();
	UFUNCTION()
	void OnRep_IsReady();
	UFUNCTION()
	void OnRep_SelectCharacter();
	UFUNCTION()
	void OnRep_ListenServerIP();
	/** Room End*/

private:
	UPROPERTY(ReplicatedUsing = OnRep_GameRoom)
	ARoom* EnteredGameRoom;

	UPROPERTY(ReplicatedUsing = OnRep_ListenServerIP)
	FString ListenServerIP;
	
	UPROPERTY(ReplicatedUsing = OnRep_RoomAdmin)
	bool IsRoomAdmin;
	
	UPROPERTY(ReplicatedUsing = OnRep_IsReady)
	bool IsReady;

	UPROPERTY(ReplicatedUsing = OnRep_SelectCharacter)
	int SelectCharacter;

	ARoom* PrevGameRoom;
	bool bSameRoomAsLocalClient;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URoomUserInfoWidgetControlle> RoomInfoWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<URoomUserInfoWidgetControlle> RoomInfoWidgetController;



};
