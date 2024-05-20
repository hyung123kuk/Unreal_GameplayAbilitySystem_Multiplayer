// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HKUIPlayerControllerBase.generated.h"

class UUserWidget;
class UHKUserWidget;
class UServerMessageWidgetController;
struct FServerMessageWidgetControllerParams;

UENUM(BlueprintType)
enum class EServerToClientMessageType : uint8
{
	None,
	//** Login Notify */
	ServerConnect,
	CheckID,
	SignUp,
	Login,
	//** Login Notify End*/

	//** Lobby Notify */
	SendChattingMessage,
	ChangeIntroduction,
	//** Lobby Notify End */

	//** Room Notify */
	MakeRoom,
	EnterRoom,
	ExitRoom,
	ChangeReadyState,
	GameStart,
	FollowRoomUser,
	//** Room Notify End*/
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageSuccessOrNotDelegate, bool, Success);
DECLARE_MULTICAST_DELEGATE_FourParams(FServerMessageDelegate,const FString&, EServerToClientMessageType, bool, bool);


UCLASS()
class UNREALPORTFOLIO_API AHKUIPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
		
public:
	virtual void BeginPlay() override;

	FServerMessageDelegate AllServerMessagesDelegate;

protected:
	UFUNCTION(Client, Reliable)
	void SendServerMessage_Client(const FString& Message, EServerToClientMessageType MessageType, bool ShowPopup, bool Success);
	virtual void ReceiveServerMessage(const FString& Message, EServerToClientMessageType MessageType, bool PopupMessage = false, bool Success = false);

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UIWidgetClass;

	UPROPERTY()
	TObjectPtr<UUserWidget> UIWidgetInstance;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UHKUserWidget> ErrorMessageClass;

	UPROPERTY()
	TObjectPtr<UHKUserWidget> ErrorMessageInstance;
	
	UPROPERTY()
	TObjectPtr<UServerMessageWidgetController> ErrorMessageWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UServerMessageWidgetController> ErrorMessageWidgetControllerClass;

};
