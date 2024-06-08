// Copyright Druid Mechanics


#include "UI/WidgetController/StoreWidgetController.h"

void UStoreWidgetController::SetWidgetControllerParams(const TArray<FStoreItemDefaultInfo>& Params)
{
	StoreItems = Params;
	ChangeStoreValueSignature.Broadcast(this);
}
