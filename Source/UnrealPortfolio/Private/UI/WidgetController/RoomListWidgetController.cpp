// Copyright Druid Mechanics


#include "UI/WidgetController/RoomListWidgetController.h"

void URoomListWidgetController::SetWidgetControllerParams(const FRoomListWidgetControllerParams& Params)
{
	RoomName = Params.RoomName;
	MaxPlayerNum = Params.MaxPlayerNum;
	NowPlayerNum = Params.NowPlayerNum;
}

FString URoomListWidgetController::GetElementName()
{
	return RoomName;
}
