// Copyright Druid Mechanics


#include "UI/WidgetController/HKRoomListWidgetController.h"

void UHKRoomListWidgetController::SetWidgetControllerParams(const FRoomListWidgetControllerParams& Params)
{
	RoomName = Params.RoomName;
	MaxPlayerNum = Params.MaxPlayerNum;
	NowPlayerNum = Params.NowPlayerNum;
}

FString UHKRoomListWidgetController::GetElementName()
{
	return RoomName;
}
