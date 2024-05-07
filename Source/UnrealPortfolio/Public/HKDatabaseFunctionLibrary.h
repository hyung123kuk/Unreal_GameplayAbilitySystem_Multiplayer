// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HKDatabaseFunctionLibrary.generated.h"

class UMySQLConnection;
class AHKLobbyGameMode;
class AHKLoginGameMode;
class AHKDedicatedServerGameModeBase;
struct FMySQLConnectoreQueryResult;

UCLASS()
class UNREALPORTFOLIO_API UHKDatabaseFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	///
	/// DataBase�� DedicatedServer������ ������ �� �����Ƿ� DedicatedServer�� ��ӹ��� GameMode������ ���� �����մϴ�.
	/// 
	friend AHKLoginGameMode;
	friend AHKLobbyGameMode;
	friend AHKDedicatedServerGameModeBase;

protected:
	static UMySQLConnection* AttemptToConnectDataBase(const FString& Host, const FString& UserName, const FString& UserPassword,const FString& DatabaseName);
	static bool CreateUserID(UMySQLConnection* Database, const FString& ID, const FString& Password);
	static bool ExistedUserID(UMySQLConnection* Database, const FString& ID);
	static bool MatchPasswordToID(UMySQLConnection* Database, const FString& ID,const FString& Password);

private:
	static FMySQLConnectoreQueryResult Query(UMySQLConnection* Database,const FString& Query);
	
	//������ ���� ������ ���� �Լ�
	static bool CheckThePossibilityOfQueryInjection(const FString& Input);
	static bool CheckThePossibilityOfQueryInjection(const FString& Input, FString& ContainedReservedWord);

};
