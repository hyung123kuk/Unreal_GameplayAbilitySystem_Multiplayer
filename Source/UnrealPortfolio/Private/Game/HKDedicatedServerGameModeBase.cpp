// Copyright Druid Mechanics


#include "Game/HKDedicatedServerGameModeBase.h"
#include "HKDatabaseFunctionLibrary.h"
#include "MySQLDatabase.h"
#include "MySQLConnection.h"
#include "UnrealPortfolio/UnrealPortfolio.h"


void AHKDedicatedServerGameModeBase::StartPlay()
{
	Super::StartPlay();

	//TEST CODE
	if (GetNetMode() == NM_Standalone)
	{
		return;
	}

	ImportMySQLAccountInformationFromJson();

}

void AHKDedicatedServerGameModeBase::ImportMySQLAccountInformationFromJson()
{
	if (!Init)
	{
		FString DatabaseInfo;

		FFileHelper::LoadFileToString(DatabaseInfo, DatabaseInfoFilePath);
		TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(DatabaseInfo);
		TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());
		FJsonSerializer::Deserialize(jsonReader, jsonObj);
		checkf(jsonObj.IsValid(), TEXT("There is no Json file to retrieve database information in path : %s"), DatabaseInfoFilePath);
		
		jsonObj->TryGetStringField(TEXT("Host"), DatabaseHost);
		jsonObj->TryGetStringField(TEXT("UserName"), DatabaseUserName);
		jsonObj->TryGetStringField(TEXT("UserPassword"), DatabaseUserPassword);
		jsonObj->TryGetStringField(TEXT("DatabaseName"), DatabaseName);
		Init = true;
	}

	UserData = UHKDatabaseFunctionLibrary::AttemptToConnectDataBase(DatabaseHost, DatabaseUserName, DatabaseUserPassword, DatabaseName);
}

bool AHKDedicatedServerGameModeBase::AttemptedToLogin(const FString& ID, const FString& Password, FString& ErrorMessage)
{
	UE_LOG(ServerLog, Warning, TEXT("유저(%s)가 로그인을 시도합니다."), *ID);

	if (!UHKDatabaseFunctionLibrary::ExistedUserID(UserData, ID))
	{
		ErrorMessage = TEXT("존재하지 않는 아이디 입니다.");
		UE_LOG(ServerLog, Error, TEXT("이 아이디(%s)는 존재하지 않는 아이디입니다."), *ID);
		return false;
	}

	if (!UHKDatabaseFunctionLibrary::MatchPasswordToID(UserData, ID, Password))
	{
		ErrorMessage = TEXT("비밀번호가 일치하지 않습니다.");
		UE_LOG(ServerLog, Error, TEXT("유저가 입력한 비밀번호가 일치하지 않습니다."));
		return false;
	}

	UE_LOG(ServerLog, Warning, TEXT("유저(%s)가 로그인에 성공했습니다."), *ID);

	return true;
}