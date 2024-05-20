// Copyright Druid Mechanics


#include "UI/WidgetController/LobbyRoomInfoWidgetController.h"

void ULobbyRoomInfoWidgetController::SetWidgetControllerParams(const FLobbyRoomInfoWidgetControllerParams& Params)
{
	RoomName = Params.RoomName;
	MaxPlayerNum = Params.MaxPlayerNum;
	NowPlayerNum = Params.NowPlayerNum;
	PublicRoom = Params.PublicRoom;
	ChangeRoomValueSignature.Broadcast(this);
}


FString ULobbyRoomInfoWidgetController::GetElementName()
{
	return RoomName;
}
