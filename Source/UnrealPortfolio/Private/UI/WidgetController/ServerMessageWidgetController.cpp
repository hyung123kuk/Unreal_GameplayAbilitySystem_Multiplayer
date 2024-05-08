// Copyright Druid Mechanics


#include "UI/WidgetController/ServerMessageWidgetController.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HKUIPlayerControllerBase.h"

void UServerMessageWidgetController::SetWidgetControllerParams(const FServerMessageWidgetControllerParams& Params)
{
	Message = Params.Message;
}
