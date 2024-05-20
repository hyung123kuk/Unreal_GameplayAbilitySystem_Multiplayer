// Copyright Druid Mechanics


#include "UI/WidgetController/UserInfoWidgetController.h"

void UUserInfoWidgetController::SetWidgetControllerParams(const FUserInfoWidgetControllerParams& Params)
{
	PlayerName = Params.PlayerName;
	EnterRoom = Params.EnterRoom;
	Introduction = Params.Introduction;
	PlayerLevel = Params.PlayerLevel;
	PlayerExp = Params.PlayerExp;
	ChangeUserInfoValueSignature.Broadcast(this);

}

FString UUserInfoWidgetController::GetElementName()
{
	return PlayerName;
}

