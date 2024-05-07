// Copyright Druid Mechanics

#include "Login/HKLoginGameMode.h"
#include "HKDatabaseFunctionLibrary.h"
#include "HKBlueprintFunctionLibrary.h"
#include "UnrealPortfolio/UnrealPortfolio.h"

bool AHKLoginGameMode::CheckIDForSignUp(const FString& ID, FString& ErrorMessage)
{
	if (UHKDatabaseFunctionLibrary::ExistedUserID(UserData, ID))
	{
		ErrorMessage = TEXT("이미 존재하는 아이디입니다.");
		UE_LOG(ServerLog, Error, TEXT("The ID that already exists... < ID : %s > "), *ID);
		return false;
	}

	if (!UHKBlueprintFunctionLibrary::CheckIDValidity(ID, ErrorMessage))
	{
		UE_LOG(ServerLog, Error, TEXT("This ID is not valid... < ID : %s > "), *ID);
		return false;
	}

	FString ReserveWord;
	if (!UHKDatabaseFunctionLibrary::CheckThePossibilityOfQueryInjection(ID, ReserveWord))
	{
		ErrorMessage = FString::Printf(TEXT("예약어는 사용할 수 없습니다. : %s "), *ReserveWord);
		UE_LOG(ServerLog, Error, TEXT("This ID is Contains reserved words. < %s > "), *ReserveWord);
		return false;
	}

	return true;
}

bool AHKLoginGameMode::CheckValidForSignUp(const FString& ID, const FString& Password, const FString& PasswordConfirm, FString& ErrorMessage)
{
	UE_LOG(ServerLog, Error, TEXT("CheckValidForSignUp"));
	if (!CheckIDForSignUp(ID, ErrorMessage))
	{
		UE_LOG(ServerLog, Error, TEXT("CheckIDForSignUp"));
		return false;
	}

	if (!UHKBlueprintFunctionLibrary::MatchPasswordAndPasswordConfirm(Password, PasswordConfirm, ErrorMessage))
	{
		UE_LOG(ServerLog, Error, TEXT("MatchPasswordAndPasswordConfirm"));
		return false;
	}

	if (!UHKBlueprintFunctionLibrary::CheckPasswordValidity(Password, ErrorMessage))
	{
		UE_LOG(ServerLog, Error, TEXT("CheckPasswordValidity"));
		return false;
	}

	if (!UHKDatabaseFunctionLibrary::CreateUserID(UserData, ID, Password))
	{
		ErrorMessage = TEXT("아이디 생성에 실패했습니다.");
		return false;
	}

	return true;
}

