// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Player/HKUIPlayerControllerBase.h"
#include "HKUILobbyPlayerController.generated.h"

class UUserWidget;
class AHKLobbyPlayerState;
class URoomUserInfoWidgetControlle;
class ULobbyRoomInfoWidgetController;
class UUserInfoWidgetController;
class UChattingWidgetController;
class UInviteRoomWidgetController;
class UStoreWidgetController;
class UInventoryWidgetController;
class ARoom;
class UInventory;
struct FInGamePlayerInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRoomUserControllerDelegate, URoomUserInfoWidgetControlle*, UserController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLobbyRoomInfoDelegate, ULobbyRoomInfoWidgetController*, RoomController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUserInfoDelegate, UUserInfoWidgetController*, UserInfoController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChattingMessageDelegate, UChattingWidgetController*, ChattingMessageController);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInviteMessageDelegate, UInviteRoomWidgetController*, InviteMessageController);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeGoldValueDelegate, int, Gold);


USTRUCT()
struct FUserItemInfo
{
	GENERATED_BODY()

	UPROPERTY()
	FString UserID;

	UPROPERTY()
	int PlayerGold;

	UPROPERTY()
	TArray<int> ItemIds;

	UPROPERTY()
	TArray<int> ItemCounts;
};

//게임 시작하기 전 같은 방에 있는 플레이어 정보를 담기위한 구조체
USTRUCT()
struct FInGamePlayerInfo
{
	GENERATED_BODY()

	UPROPERTY()
	int UserOrder;

	UPROPERTY()
	FString UserID;

	UPROPERTY()
	FString UserIP;

	UPROPERTY()
	bool RoomAdmin;

	UPROPERTY()
	FUserItemInfo UserItemInfo;
};


UCLASS()
class UNREALPORTFOLIO_API AHKUILobbyPlayerController : public AHKUIPlayerControllerBase
{
	GENERATED_BODY()
	
public:

	//** From Client */
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToMakeRoomToServer(const FString& RoomName, const FString& RoomPassword, int MaxPlayers);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToEnteredRoomToServer(const FString& RoomName, const FString& RoomPassword);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToExitRoomToServer(const FString& RoomName);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToSendChattingMessageToServer(const FString& Message);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToChangeUserIntroductionMessageToServer(const FString& Introduction);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToChangeReadyState(bool IsReady);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToGameStart(const FString& RoomName);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToFollowRoomUser(const FString& UserToFollow, const FString& RoomPassword);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToInviteLobbyUser(const FString& UserToInvite);
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void TryToPurchaseItem(int ItemID);
	//** From Client End */

	//** From Server */
	UFUNCTION(Client, Reliable)
	void NotifyReceiveChattingMessageToClient(const FString& SendUserName, const FString& ChattingMessage);
	UFUNCTION(Client, Reliable)
	void NotifyInviteRoomMessageToClient(const FString& SendUserName,const FString& InvitedRoom,const FString& RoomPassword);
	//** From Server End*/

	//** Lobby UI */
	void SetMyUserInfoWidgetController(UUserInfoWidgetController* UserInfoWidgetController);
	void EnterLobbyUserWidgetController(UUserInfoWidgetController* EnterUserInfoController);
	void ExitLobbyUserWidgetController(UUserInfoWidgetController* ExitUserInfoController);
	void MakeLobbyRoomWidgetController(ULobbyRoomInfoWidgetController* RoomInfoController);
	void RemoveLobbyRoomWidgetController(ULobbyRoomInfoWidgetController* RoomInfoController);
	void SetStoreWidgetController(UStoreWidgetController* StoreInfoController);
	void SetInventoryWidgetController(UInventoryWidgetController* InventoryInfoController);
	//** Lobby UI End*/

	//** Popup Room UI */
	void CreateAndShowRoomWidget();
	void EnterSameRoomUserWidgetController(URoomUserInfoWidgetControlle* EnterUserInfoController);
	void ExitGameRoomUserWidgetController(URoomUserInfoWidgetControlle* ExitUserInfoController);
	//** Popup Room UI End*/

	/** Game Start Logic */
	UFUNCTION(Client, Reliable)
	void StoreInGamePlayerInfoAndGameStart(const TArray<FInGamePlayerInfo>& InGamePlayersInfo, const FString& AdminIP);
	/** Game Start Logic End*/


protected:
	virtual void ReceiveServerMessage(const FString& Message, EServerToClientMessageType MessageType, bool PopupMessage = false, bool Success = false) override;

public:
	//** Notify SuccessOrNot From Server */
	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Lobby")
	FMessageSuccessOrNotDelegate SendChattingMessageSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Lobby")
	FMessageSuccessOrNotDelegate ChangeIntroductionMessageSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Room")
	FMessageSuccessOrNotDelegate MakeRoomSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Room")
	FMessageSuccessOrNotDelegate EnterRoomSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Room")
	FMessageSuccessOrNotDelegate ExitRoomSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Room")
	FMessageSuccessOrNotDelegate ChangeReadyStateSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Room")
	FMessageSuccessOrNotDelegate GameStartSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Room")
	FMessageSuccessOrNotDelegate FollowRoomUserSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Room")
	FMessageSuccessOrNotDelegate InviteLobbyUserSuccessOrNotDelegate;

	UPROPERTY(BlueprintAssignable, Category = "SuccessOrNot||Store")
	FMessageSuccessOrNotDelegate PurchaseItemSuccessOrNotDelegate;
	//** Notify SuccessOrNot From Server End*/

	//** Lobby UI */
	UPROPERTY(BlueprintAssignable, Category = "Lobby")
	FUserInfoDelegate MyUserInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
	FUserInfoDelegate EnterLobbyNewUserDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
	FUserInfoDelegate ExitLobbyUserDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
	FLobbyRoomInfoDelegate MakeNewRoomDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
	FLobbyRoomInfoDelegate RemoveRoomDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Lobby")
	FLobbyRoomInfoDelegate ChangeRoomInfoDelegate;

	//** Lobby UI End*/

	//** From Server */
	UPROPERTY(BlueprintAssignable, Category = "Chatting")
	FChattingMessageDelegate RecieveChattingMessageDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Chatting")
	FInviteMessageDelegate RecieveInviteMessageDelegate;
	//** From Server End*/

	//** Room UI */
	UPROPERTY(BlueprintAssignable, Category = "Room")
	FRoomUserControllerDelegate EnterRoomNewUserDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Room")
	FRoomUserControllerDelegate ExitRoomUserDelegate;
	//** Room UI End*/

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UIPopupRoomClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> UIPopupRoomInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UIPopupStoreClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> UIPopupStoreInstance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStoreWidgetController> StoreInfoWidgetController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UChattingWidgetController> ChattingWidgetControllerClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UInviteRoomWidgetController> InviteWidgetControllerClass;

public:
	UFUNCTION(BlueprintCallable)
	int GetGold() { return ItemInfo.PlayerGold; }

	UFUNCTION()
	void OnRep_ItemInfo();

	UPROPERTY(ReplicatedUsing = OnRep_ItemInfo)
	FUserItemInfo ItemInfo;

	UPROPERTY(BlueprintAssignable, Category = "PlayerInfo")
	FChangeGoldValueDelegate ChangeGoldDelegate;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInventory> InventoryClass;

	UPROPERTY()
	TObjectPtr<UInventory> Inventory;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
