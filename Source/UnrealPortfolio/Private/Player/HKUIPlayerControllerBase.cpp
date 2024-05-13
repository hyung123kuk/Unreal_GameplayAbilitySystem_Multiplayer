// Copyright Druid Mechanics


#include "Player/HKUIPlayerControllerBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Game/HKGameInstance.h"
#include "UI/HKUserWidget.h"
#include "UI/WidgetController/ServerMessageWidgetController.h"

void AHKUIPlayerControllerBase::BeginPlay()
{
    Super::BeginPlay();

    if (GetNetMode() == NM_DedicatedServer)
    {
        return;
    }

    if (IsValid(UIWidgetClass))
    {
        UIWidgetInstance = CreateWidget<UUserWidget>(this, UIWidgetClass);
        if (IsValid(UIWidgetInstance))
        {
            UIWidgetInstance->AddToViewport();
            bShowMouseCursor = true;
        }
    }

    //서버 접속 실패 알람 확인
    UHKGameInstance* GameInstace = Cast<UHKGameInstance>(GetGameInstance());
    FString ErrorMessage;
    if (GameInstace->GetNetworkLevelChangeErrorMessage(ErrorMessage))
    {
        ReceiveServerMessage(ErrorMessage, EServerToClientMessageType::ServerConnect , true, false);
    }

}

void AHKUIPlayerControllerBase::SendServerMessage_Client_Implementation(const FString& Message, EServerToClientMessageType MessageType, bool ShowPopup, bool bSuccess)
{
    ReceiveServerMessage(Message, MessageType, ShowPopup, bSuccess);
}

void AHKUIPlayerControllerBase::ReceiveServerMessage(const FString& Message, EServerToClientMessageType MessageType,bool PopupMessage, bool Success)
{
    //에러 메세지
    if (PopupMessage && IsValid(ErrorMessageClass))
    {
        ErrorMessageInstance = CreateWidget<UHKUserWidget>(this, ErrorMessageClass);
        ErrorMessageWidgetController = NewObject<UServerMessageWidgetController>(this, ErrorMessageWidgetControllerClass);
        ErrorMessageWidgetController->SetWidgetControllerParams(FServerMessageWidgetControllerParams(Message));
        ErrorMessageInstance->SetWidgetController(ErrorMessageWidgetController);
        if (IsValid(UIWidgetInstance))
        {
            ErrorMessageInstance->AddToViewport();
        }
    }

    AllServerMessagesDelegate.Broadcast(Message, MessageType, PopupMessage, Success);

}
