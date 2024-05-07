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

bool AHKDedicatedServerGameModeBase::AttemptedToLogin(const FString& Id, const FString& Password, FString& ErrorMessage)
{
	UE_LOG(ServerLog, Warning, TEXT("Player Login Attempt < ID : %s Password : %s > "), *Id, *Password);

	bool ExistedID = UHKDatabaseFunctionLibrary::ExistedUserID(UserData, Id);

	if (!ExistedID)
	{
		ErrorMessage = TEXT("The created ID does not exist...");
		return false;
	}

	bool MatchPasswordToID = UHKDatabaseFunctionLibrary::MatchPasswordToID(UserData, Id, Password);
	if (!MatchPasswordToID)
	{
		ErrorMessage = TEXT("Passwords do not match..");
		return false;
	}

	//TEST CODE (임시 주석)
	/*if (AllPlayers.Contains(Id))
	{
		ErrorMessage = TEXT("Already logged in..");
		return false;
	}*/

	return true;
}