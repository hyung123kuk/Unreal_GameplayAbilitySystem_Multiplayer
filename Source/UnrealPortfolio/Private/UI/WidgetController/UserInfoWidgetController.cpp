// Copyright Druid Mechanics


#include "UI/WidgetController/UserInfoWidgetController.h"

void UUserInfoWidgetController::SetWidgetControllerParams(const FUserInfoWidgetControllerParams& Params)
{
	PlayerName = Params.PlayerName;
	EnterRoomName = Params.EnterRoomName;
	Introduction = Params.Introduction;
	PlayerLevel = Params.PlayerLevel;
	PlayerExp = Params.PlayerExp;
	ChangeUserInfoValueSignature.Broadcast();

}

FString UUserInfoWidgetController::GetElementName()
{
	return PlayerName;
}

