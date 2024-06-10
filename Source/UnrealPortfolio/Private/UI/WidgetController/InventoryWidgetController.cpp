// Copyright Druid Mechanics


#include "UI/WidgetController/InventoryWidgetController.h"

void UInventoryWidgetController::SetWidgetControllerParams(const FInventoryWidgetControllerParams& Params)
{
	ConsumableItems = Params.ConsumableItems;
	Characters = Params.Characters;
	ChangeInventoryValueSignature.Broadcast(this);
}
