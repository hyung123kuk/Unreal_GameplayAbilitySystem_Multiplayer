// Copyright Druid Mechanics

#include "Login/HKLoginGameMode.h"
#include "HKDatabaseFunctionLibrary.h"
#include "HKBlueprintFunctionLibrary.h"
#include "UnrealPortfolio/UnrealPortfolio.h"


bool AHKLoginGameMode::CheckIDForSignUp(const FString& ID, FString& ErrorMessage)
{
	UE_LOG(ServerLog, Log, TEXT("유저가 아이디 확인을(%s) 시도합니다 "), *ID);
	if (UHKDatabaseFunctionLibrary::ExistedUserID(UserData, ID))
	{
		ErrorMessage = TEXT("이미 존재하는 아이디입니다.");
		UE_LOG(ServerLog, Error, TEXT("유저가 이미 존재하는 아이디(%s)로 회원가입을 시도하여 회원가입에 실패합니다."), *ID);
		return false;
	}

	if (!UHKBlueprintFunctionLibrary::CheckIDValidity(ID, ErrorMessage))
	{
		UE_LOG(ServerLog, Error, TEXT("유저가 타당하지 않은 아이디(%s)로 회원가입을 시도하여 회원가입에 실패합니다."), *ID);
		return false;
	}

	FString ReserveWord;
	if (!UHKDatabaseFunctionLibrary::CheckThePossibilityOfQueryInjection(ID, ReserveWord))
	{
		ErrorMessage = FString::Printf(TEXT("예약어는 사용할 수 없습니다. : %s "), *ReserveWord);
		UE_LOG(ServerLog, Error, TEXT("유저가 예약어(%s)를 포함하는 아이디(%s)로 방을 만들어 회원가입에 실패합니다."), *ReserveWord, *ID);
		return false;
	}
	UE_LOG(ServerLog, Log, TEXT("유저는 아이디(%s)를 사용할 수 있습니다."), *ID);
	return true;
}

bool AHKLoginGameMode::CheckValidForSignUp(const FString& ID, const FString& Password, const FString& PasswordConfirm, FString& ErrorMessage)
{

	if (!CheckIDForSignUp(ID, ErrorMessage))
	{
		return false;
	}

	if (!UHKBlueprintFunctionLibrary::MatchPasswordAndPasswordConfirm(Password, PasswordConfirm, ErrorMessage))
	{
		return false;
	}

	if (!UHKBlueprintFunctionLibrary::CheckPasswordValidity(Password, ErrorMessage))
	{
		return false;
	}

	if (!UHKDatabaseFunctionLibrary::CreateUserID(UserData, ID, Password))
	{
		ErrorMessage = TEXT("아이디 생성에 실패했습니다.");
		return false;
	}

	return true;
}


