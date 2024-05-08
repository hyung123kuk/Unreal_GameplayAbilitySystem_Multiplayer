// Copyright Druid Mechanics


#include "UI/HKGASWidgetController.h"

void UHKGASWidgetController::SetWidgetControllerParams(const FGASWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}
