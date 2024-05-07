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
	ServerConnect,
	CheckID,
	SignUp
};

DECLARE_MULTICAST_DELEGATE_FourParams(FServerMessageDelegate,const FString&, EServerToClientMessageType, bool, bool);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageSuccessOrNotDelegate, bool, Success);

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKUIPlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
		
public:
	virtual void BeginPlay() override;
	virtual void ReceiveServerMessage(const FString& Message, EServerToClientMessageType MessageType ,bool PopupMessage = false, bool Success = false);

	FServerMessageDelegate AllServerMessagesDelegate;

private:
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
