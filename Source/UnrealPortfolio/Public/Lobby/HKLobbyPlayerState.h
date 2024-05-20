// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Lobby/HKLobbyGameMode.h"
#include "HKLobbyPlayerState.generated.h"

class URoomUserInfoWidgetControlle;
class UUserInfoWidgetController;
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
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	/** Only Server Func */
	void SetEnteredRoom(ARoom* EnterGameRoom) { EnteredGameRoom = EnterGameRoom; }
	void SetIsRoomAdmin(bool bIsRoomAdmin) { IsRoomAdmin = bIsRoomAdmin; }
	void SetIntroduction(FString NewIntroduction) { Introduction = NewIntroduction; }
	void SetGold (int SetPlayerGold) { PlayerGold = SetPlayerGold; }
	void SetExp (int SetExp) { PlayerExp = SetExp; }
	void SetListenServerIP(FString ServerIP) { ListenServerIP = ServerIP; }
	void SetIsReady(bool bIsReady) { IsReady = bIsReady; }
	/** Only Server Func End*/

	/** Room Info Get*/
	UFUNCTION(BlueprintCallable)
	ARoom* GetEnteredRoom() const { return EnteredGameRoom; }
	UFUNCTION(BlueprintCallable)
	bool GetIsReady() const { return IsReady; }
	UFUNCTION(BlueprintCallable)
	bool GetIsRoomAdmin() const { return IsRoomAdmin; }
	/** Room Info Get End*/

	/** Match Making */
	UFUNCTION(BlueprintCallable)
	const FString& GetListenServerIP() const { return ListenServerIP; }
	/** Match Making End */

	void GameStart();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	/** Lobby */
	void EnterLobbyNewPlayer();
	void ExitLobbyPlayer();
	void SendChangePlayerInformationToLocalClient();

	UFUNCTION()
	void OnRep_Introduction();
	UFUNCTION()
	void OnRep_Level();
	UFUNCTION()
	void OnRep_Exp();
	UFUNCTION()
	void OnRep_Gold();
	/** Lobby End*/

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

	/** Lobby */
	UPROPERTY(ReplicatedUsing = OnRep_Introduction)
	FString Introduction;

	UPROPERTY(ReplicatedUsing = OnRep_Level)
	int PlayerLevel;

	UPROPERTY(ReplicatedUsing = OnRep_Exp)
	int PlayerExp;

	UPROPERTY(ReplicatedUsing = OnRep_Gold)
	int PlayerGold;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserInfoWidgetController> UserInfoWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UUserInfoWidgetController> UserInfoWidgetController;
	/** Lobby End*/

private:

	/** Room */
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
	/** Room End*/

};
