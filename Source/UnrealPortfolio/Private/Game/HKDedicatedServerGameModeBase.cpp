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
	FString DatabaseInfo;

	FFileHelper::LoadFileToString(DatabaseInfo, DatabaseInfoFilePath);
	TSharedRef<TJsonReader<TCHAR>> jsonReader = TJsonReaderFactory<TCHAR>::Create(DatabaseInfo);
	TSharedPtr<FJsonObject> jsonObj = MakeShareable(new FJsonObject());

	FString DatabaseHost;
	FString DatabaseUserName;
	FString DatabaseUserPassword;
	FString DatabaseName;
	if (FJsonSerializer::Deserialize(jsonReader, jsonObj) && jsonObj.IsValid())
	{
		jsonObj->TryGetStringField(TEXT("Host"), DatabaseHost);
		jsonObj->TryGetStringField(TEXT("UserName"), DatabaseUserName);
		jsonObj->TryGetStringField(TEXT("UserPassword"), DatabaseUserPassword);
		jsonObj->TryGetStringField(TEXT("DatabaseName"), DatabaseName);

		UserData = UHKDatabaseFunctionLibrary::AttemptToConnectDataBase(DatabaseHost, DatabaseUserName, DatabaseUserPassword, DatabaseName);
	}
	else
	{
		checkf(false, TEXT("There is no Json file to retrieve database information in path : %s"), DatabaseInfoFilePath);
	}
}

bool AHKDedicatedServerGameModeBase::AttemptedToLogin(const FString& ID, const FString& Password, FString& ErrorMessage)
{
	UE_LOG(ServerLog, Warning, TEXT("Player Login Attempt < ID : %s Password : %s > "), *ID, *Password);

	if (!UHKDatabaseFunctionLibrary::ExistedUserID(UserData, ID))
	{
		ErrorMessage = TEXT("존재하지 않는 아이디 입니다.");
		UE_LOG(ServerLog, Error, TEXT("This ID does not exist < ID : %s > "), *ID);
		return false;
	}

	if (!UHKDatabaseFunctionLibrary::MatchPasswordToID(UserData, ID, Password))
	{
		ErrorMessage = TEXT("비밀번호가 일치하지 않습니다.");
		UE_LOG(ServerLog, Error, TEXT("Passwords do not match < ID : %s > "), *ID);
		return false;
	}

	return true;
}