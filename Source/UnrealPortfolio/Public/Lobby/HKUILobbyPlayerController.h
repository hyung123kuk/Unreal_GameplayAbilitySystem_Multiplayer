// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Player/HKUIPlayerControllerBase.h"
#include "HKUILobbyPlayerController.generated.h"

class UUserWidget;
class AHKLobbyPlayerState;
class URoomUserInfoWidgetControlle;
class ULobbyRoomInfoWidgetController;
class ARoom;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoomUserControllerDelegate, URoomUserInfoWidgetControlle*, UserController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLobbyRoomInfoDelegate, ULobbyRoomInfoWidgetController*, RoomController);


UCLASS()
class UNREALPORTFOLIO_API AHKUILobbyPlayerController : public AHKUIPlayerControllerBase
{
	GENERATED_BODY()
	
public:
	//** From Client */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToMakeRoomToServer(const FString& UserName, const FString& RoomName, const FString& RoomPassword, int MaxPlayers);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToEnteredRoomToServer(const FString& UserName, const FString& RoomName, const FString& RoomPassword);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToExitRoomToServer(const FString& UserName, const FString& RoomName);
	//** From Client End */

	//** Lobby UI */
	void MakeLobbyRoomWidgetController(ULobbyRoomInfoWidgetController* RoomInfoController);
	void RemoveLobbyRoomWidgetController(ULobbyRoomInfoWidgetController* RoomInfoController);
	//** Lobby UI End*/

	//** Popup Room UI */
	void CreateAndShowRoomWidget();
	void EnterSameRoomUserWidgetController(URoomUserInfoWidgetControlle* EnterUserInfoController);
	void ExitGameRoomUserWidgetController(URoomUserInfoWidgetControlle* ExitUserInfoController);
	//** Popup Room UI End*/

protected:
	virtual void ReceiveServerMessage(const FString& Message, EServerToClientMessageType MessageType, bool PopupMessage = false, bool Success = false) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Room")
	FMessageSuccessOrNotDelegate MakeRoomSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Room")
	FMessageSuccessOrNotDelegate EnterRoomSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Room")
	FMessageSuccessOrNotDelegate ExitRoomSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "ChangeRoomInfo")
	FLobbyRoomInfoDelegate MakeNewRoomDelegate;

	UPROPERTY(BlueprintAssignable, Category = "ChangeRoomInfo")
	FLobbyRoomInfoDelegate RemoveRoomDelegate;

	UPROPERTY(BlueprintAssignable, Category = "ChangeRoomInfo")
	FLobbyRoomInfoDelegate ChangeRoomInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "ChangeRoomInfo")
	FRoomUserControllerDelegate EnterRoomNewUserDelegate;

	UPROPERTY(BlueprintAssignable, Category = "ChangeRoomInfo")
	FRoomUserControllerDelegate ExitRoomUserDelegate;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UIPopupRoomClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> UIPopupRoomInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UIPopupStoreClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> UIPopupStoreInstance;

};
