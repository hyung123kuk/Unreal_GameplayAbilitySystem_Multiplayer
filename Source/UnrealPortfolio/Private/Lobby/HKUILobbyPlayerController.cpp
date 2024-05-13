// Copyright Druid Mechanics


#include "Lobby/HKUILobbyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Lobby/HKLobbyGameMode.h"
#include "Lobby/HKLobbyPlayerState.h"
#include "UI/WidgetController/RoomUserInfoWidgetControlle.h"

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
}


