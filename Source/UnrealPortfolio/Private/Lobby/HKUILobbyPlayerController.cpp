// Copyright Druid Mechanics


#include "Lobby/HKUILobbyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Lobby/HKLobbyGameMode.h"
#include "Lobby/HKLobbyPlayerState.h"
#include "UI/WidgetController/RoomUserInfoWidgetControlle.h"
#include "UI/WidgetController/ChattingWidgetController.h"

void AHKUILobbyPlayerController::TryToMakeRoomToServer_Implementation(const FString& UserName, const FString& RoomName, const FString& RoomPassword, int MaxPlayers)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString Message = FString();
    bool bSuccess = false;
    if (GameMode)
    {
        if (GameMode->TryToMakeAndEnterRoom(UserName, RoomName, RoomPassword, MaxPlayers, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::MakeRoom, !bSuccess, bSuccess);
}

void AHKUILobbyPlayerController::TryToEnteredRoomToServer_Implementation(const FString& UserName, const FString& RoomName, const FString& RoomPassword)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString Message = FString();
    bool bSuccess = false;
    if (GameMode)
    {
        if (GameMode->TryToEnterRoom(UserName, RoomName, RoomPassword, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::EnterRoom, !bSuccess, bSuccess);
}

void AHKUILobbyPlayerController::TryToExitRoomToServer_Implementation(const FString& UserName, const FString& RoomName)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString Message = FString();
    bool bSuccess = false;
    if (GameMode)
    {
        if (GameMode->TryToExitRoomAndGoToLobby(UserName, RoomName, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::ExitRoom, !bSuccess, bSuccess);
}

void AHKUILobbyPlayerController::TryToSendChattingMessageToServer_Implementation(const FString& UserName, const FString& ChatMessage)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString Message = FString();
    bool bSuccess = false;
    if (GameMode)
    {
        if (GameMode->TryToSendMessageOtherClients(UserName, ChatMessage, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::SendChattingMessage, !bSuccess, bSuccess);
}

void AHKUILobbyPlayerController::TryToChangeUserIntroductionMessageToServer_Implementation(const FString& UserName, const FString& Introduction)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString IntroductionCopy = Introduction;
    FString Message = FString();
    bool bSuccess = false;
    if (GameMode)
    {
        if (GameMode->TryToChangeIntroductionMessage(UserName, IntroductionCopy, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::ChangeIntroduction, !bSuccess, bSuccess);
}

void AHKUILobbyPlayerController::SetMyUserInfoWidgetController(UUserInfoWidgetController* UserInfoWidgetController)
{
    MyUserInfoDelegate.Broadcast(UserInfoWidgetController);
}

void AHKUILobbyPlayerController::EnterLobbyUserWidgetController(UUserInfoWidgetController* EnterUserInfoController)
{
    EnterLobbyNewUserDelegate.Broadcast(EnterUserInfoController);
}

void AHKUILobbyPlayerController::ExitLobbyUserWidgetController(UUserInfoWidgetController* ExitUserInfoController)
{
    ExitLobbyUserDelegate.Broadcast(ExitUserInfoController);
}

void AHKUILobbyPlayerController::MakeLobbyRoomWidgetController(ULobbyRoomInfoWidgetController* RoomInfoController)
{
    MakeNewRoomDelegate.Broadcast(RoomInfoController);
}

void AHKUILobbyPlayerController::RemoveLobbyRoomWidgetController(ULobbyRoomInfoWidgetController* RoomInfoController)
{
    RemoveRoomDelegate.Broadcast(RoomInfoController);
}

void AHKUILobbyPlayerController::NotifyReceiveChattingMessageToClient_Implementation(const FString& SendUserName, const FString& ChattingMessage)
{
    UChattingWidgetController* ChattingWidgetController = NewObject<UChattingWidgetController>(this, ChattingWidgetControllerClass);
    if (ChattingWidgetController != nullptr)
    {
        ChattingWidgetController->SetWidgetControllerParams(FChattingWidgetControllerParams(SendUserName, ChattingMessage));
    }
    RecieveChattingMessageDelegate.Broadcast(ChattingWidgetController);
}

void AHKUILobbyPlayerController::CreateAndShowRoomWidget()
{
    if (UIPopupRoomInstance == nullptr)
    {
        UIPopupRoomInstance = CreateWidget<UUserWidget>(GetWorld(), UIPopupRoomClass);
    }

    if (IsValid(UIPopupRoomInstance))
        UIPopupRoomInstance->AddToViewport();
}

void AHKUILobbyPlayerController::EnterSameRoomUserWidgetController(URoomUserInfoWidgetControlle* EnterUserInfoController)
{
    EnterRoomNewUserDelegate.Broadcast(EnterUserInfoController);
}

void AHKUILobbyPlayerController::ExitGameRoomUserWidgetController(URoomUserInfoWidgetControlle* ExitUserInfoController)
{
    ExitRoomUserDelegate.Broadcast(ExitUserInfoController);
}

void AHKUILobbyPlayerController::ReceiveServerMessage(const FString& Message, EServerToClientMessageType MessageType, bool PopupMessage, bool Success)
{
    Super::ReceiveServerMessage(Message, MessageType, PopupMessage, Success);

    if (MessageType == EServerToClientMessageType::MakeRoom)
    {
        MakeRoomSuccessOrNotDelegate.Broadcast(Success);
    }
    if (MessageType == EServerToClientMessageType::EnterRoom)
    {
        EnterRoomSuccessOrNotDelegate.Broadcast(Success);
    }
    if (MessageType == EServerToClientMessageType::ExitRoom)
    {
        ExitRoomSuccessOrNotDelegate.Broadcast(Success);
    }
    if (MessageType == EServerToClientMessageType::SendChattingMessage)
    {
        SendChattingMessageSuccessOrNotDelegate.Broadcast(Success);
    }
    if (MessageType == EServerToClientMessageType::ChangeIntroduction)
    {
        ChangeIntroductionMessageSuccessOrNotDelegate.Broadcast(Success);
    }
}


