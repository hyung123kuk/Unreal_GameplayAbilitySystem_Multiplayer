// Copyright Druid Mechanics


#include "UI/WidgetController/InviteRoomWidgetController.h"

void UInviteRoomWidgetController::SetWidgetControllerParams(const FInviteRoomWidgetControllerParams& Params)
{
	SendUserName = Params.SendUserName;
	RoomName = Params.RoomName;
	RoomPassword = Params.RoomPassword;
}
