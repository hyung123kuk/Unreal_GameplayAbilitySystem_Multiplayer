// Copyright Druid Mechanics


#include "UI/WidgetController/UserInfoWidgetController.h"

void UUserInfoWidgetController::SetWidgetControllerParams(const FUserInfoWidgetControllerParams& Params)
{
	PlayerName = Params.PlayerName;
	EnterRoomName = Params.EnterRoomName;
	Introduction = Params.Introduction;
	PlayerLevel = Params.PlayerLevel;
	PlayerExe = Params.PlayerExe;
}

FString UUserInfoWidgetController::GetElementName()
{
	return PlayerName;
}

