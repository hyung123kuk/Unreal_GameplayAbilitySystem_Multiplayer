// Copyright Druid Mechanics


#include "UI/HKUserWidget.h"

void UHKUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}

