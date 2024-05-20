// Copyright Druid Mechanics


#include "Lobby/HKUILobbyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Lobby/HKLobbyGameMode.h"
#include "Lobby/HKLobbyPlayerState.h"
#include "UI/WidgetController/RoomUserInfoWidgetControlle.h"
#include "UI/WidgetController/ChattingWidgetController.h"

void AHKUILobbyPlayerController::TryToMakeRoomToServer_Implementation(const FString& RoomName, const FString& RoomPassword, int MaxPlayers)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString Message = FString();
    bool bSuccess = false;
    
    if (GameMode)
    {
        if (GameMode->TryToMakeAndEnterRoom(*this, RoomName, RoomPassword, MaxPlayers, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::MakeRoom, !bSuccess, bSuccess);
}

void AHKUILobbyPlayerController::TryToEnteredRoomToServer_Implementation(const FString& RoomName, const FString& RoomPassword)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString Message = FString();
    bool bSuccess = false;
    if (GameMode)
    {
        if (GameMode->TryToEnterRoom(*this, RoomName, RoomPassword, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::EnterRoom, !bSuccess, bSuccess);
}

void AHKUILobbyPlayerController::TryToExitRoomToServer_Implementation(const FString& RoomName)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString Message = FString();
    bool bSuccess = false;
    if (GameMode)
    {
        if (GameMode->TryToExitRoomAndGoToLobby(*this, RoomName, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::ExitRoom, !bSuccess, bSuccess);
}

void AHKUILobbyPlayerController::TryToSendChattingMessageToServer_Implementation(const FString& ChatMessage)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString Message = FString();
    bool bSuccess = false;
    if (GameMode)
    {
        if (GameMode->TryToSendMessageOtherClients(*this, ChatMessage, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::SendChattingMessage, !bSuccess, bSuccess);
}

void AHKUILobbyPlayerController::TryToChangeUserIntroductionMessageToServer_Implementation(const FString& Introduction)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString IntroductionCopy = Introduction;
    FString Message = FString();
    bool bSuccess = false;
    if (GameMode)
    {
        if (GameMode->TryToChangeIntroductionMessage(*this, IntroductionCopy, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::ChangeIntroduction, !bSuccess, bSuccess);
}

void AHKUILobbyPlayerController::TryToChangeReadyState_Implementation(bool IsReady)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString Message = FString();
    bool bSuccess = false;
    if (GameMode)
    {
        if (GameMode->TryToChangeReadyState(*this, IsReady, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::ChangeReadyState, !bSuccess, bSuccess);
}

void AHKUILobbyPlayerController::TryToGameStart_Implementation(const FString& RoomName)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString Message = FString();
    bool bSuccess = false;
    if (GameMode)
    {
        if (GameMode->TryToGameStart(*this, RoomName, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::GameStart, !bSuccess, bSuccess);
}

void AHKUILobbyPlayerController::TryToFollowRoomUser_Implementation(const FString& UserToFollow, const FString& RoomPassword)
{
    AHKLobbyGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLobbyGameMode>();
    FString Message = FString();
    bool bSuccess = false;
    if (GameMode)
    {
        if (GameMode->TryToFollowRoomUser(*this, UserToFollow, RoomPassword, Message))
        {
            bSuccess = true;
        }
    }

    SendServerMessage_Client(Message, EServerToClientMessageType::FollowRoomUser, !bSuccess, bSuccess);
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

    //** Lobby Notify */
    if (MessageType == EServerToClientMessageType::SendChattingMessage)
    {
        SendChattingMessageSuccessOrNotDelegate.Broadcast(Success);
    }
    if (MessageType == EServerToClientMessageType::ChangeIntroduction)
    {
        ChangeIntroductionMessageSuccessOrNotDelegate.Broadcast(Success);
    }
    //** Lobby Notify End */

    //** Room Notify */
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
    if (MessageType == EServerToClientMessageType::ChangeReadyState)
    {
        ChangeReadyStateSuccessOrNotDelegate.Broadcast(Success);
    }
    if (MessageType == EServerToClientMessageType::GameStart)
    {
        GameStartSuccessOrNotDelegate.Broadcast(Success);
    }
    if (MessageType == EServerToClientMessageType::FollowRoomUser)
    {
        FollowRoomUserSuccessOrNotDelegate.Broadcast(Success);
    }
    //** Room Notify End*/
}


