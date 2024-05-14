// Copyright Druid Mechanics


#include "UI/WidgetController/RoomUserInfoWidgetControlle.h"

void URoomUserInfoWidgetControlle::SetWidgetControllerParams(const FRoomUserInfoWidgetControllerParams& Params)
{
	PlayerName = Params.PlayerName;
	RoomAdmin = Params.RoomAdmin;
	IsReady = Params.IsReady;
	SelectCharacter = Params.SelectCharacter;
	ChangeRoomValueSignature.Broadcast();
}


FString URoomUserInfoWidgetControlle::GetElementName()
{
	return PlayerName;
}
