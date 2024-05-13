// Copyright Druid Mechanics


#include "Login/HKUILoginController.h"
#include "Net/UnrealNetwork.h"
#include "Login/HKLoginGameMode.h"
#include "UnrealPortfolio/UnrealPortfolio.h"


void AHKUILoginController::CheckIDForSignUp_Server_Implementation(const FString& ID)
{
	AHKLoginGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLoginGameMode>();
	FString Message = FString();
	bool bSuccess = false;
	if (GameMode)
	{
		if (GameMode->CheckIDForSignUp(ID, Message))
		{
			Message = TEXT("사용할 수 있는 ID입니다.");
			bSuccess = true;
			UE_LOG(ServerLog, Log, TEXT("유저가 사용할수 있는 아이디(%s) 확인에 성공했습니다."), *ID);
		}
	}

	SendServerMessage_Client(Message, EServerToClientMessageType::CheckID, true, bSuccess);
}

void AHKUILoginController::SignUp_Server_Implementation(const FString& ID, const FString& Password, const FString& PasswordConfirm)
{
	AHKLoginGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLoginGameMode>();
	FString Message = FString();
	bool bSuccess = false;
	if (GameMode)
	{
		if (GameMode->CheckValidForSignUp(ID, Password, PasswordConfirm, Message))
		{
			Message = TEXT("회원가입에 성공했습니다.");
			bSuccess = true;
		}
	}

	SendServerMessage_Client(Message, EServerToClientMessageType::SignUp, true, bSuccess);
}


void AHKUILoginController::AttempLogin_Server_Implementation(const FString& ID, const FString& Password)
{
	AHKLoginGameMode* GameMode = GetWorld()->GetAuthGameMode<AHKLoginGameMode>();
	FString Message = FString();
	bool bSuccess = false;
	if (GameMode)
	{
		if (GameMode->AttemptedToLogin(ID, Password, Message))
		{
			bSuccess = true;
		}
	}

	SendServerMessage_Client(Message, EServerToClientMessageType::Login, !bSuccess, bSuccess);
}

void AHKUILoginController::ReceiveServerMessage(const FString& Message, EServerToClientMessageType MessageType, bool PopupMessage, bool Success)
{
	Super::ReceiveServerMessage(Message, MessageType, PopupMessage, Success);

	if (MessageType == EServerToClientMessageType::CheckID)
	{
		CheckIDSuccessOrNotDelegate.Broadcast(Success);
	}

	if (MessageType == EServerToClientMessageType::Login)
	{
		LoginSuccessOrNotDelegate.Broadcast(Success);
	}
}