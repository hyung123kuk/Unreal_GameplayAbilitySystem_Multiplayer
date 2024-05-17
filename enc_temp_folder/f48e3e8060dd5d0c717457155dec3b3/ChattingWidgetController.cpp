// Copyright Druid Mechanics


#include "UI/WidgetController/ChattingWidgetController.h"

void UChattingWidgetController::SetWidgetControllerParams(const FChattingWidgetControllerParams& Params)
{
	UserName = Params.UserName;
	Chat = Params.Chat;
}

FString UChattingWidgetController::GetElementName()
{
	//ä�� ������ �ϳ��� �߰�
	static int ChatOrder = 0;

	FString ChatOrderString = FString::FromInt(ChatOrder);
	ChatOrder++;
	return ChatOrderString;
}
