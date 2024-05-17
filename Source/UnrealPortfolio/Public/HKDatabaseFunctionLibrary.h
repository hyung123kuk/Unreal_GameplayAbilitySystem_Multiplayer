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
	/** Login */
	static UMySQLConnection* AttemptToConnectDataBase(const FString& Host, const FString& UserName, const FString& UserPassword,const FString& DatabaseName);
	static bool CreateUserID(UMySQLConnection* Database, const FString& ID, const FString& Password);
	static bool ExistedUserID(UMySQLConnection* Database, const FString& ID);
	static bool MatchPasswordToID(UMySQLConnection* Database, const FString& ID,const FString& Password);
	/** Login End*/

	/** Lobby */
	static bool GetUserInformation(UMySQLConnection* Database, const FString& ID, FString& Introduction, int& Gold, int& Exp);
	static bool RecordChatInDatabase(UMySQLConnection* Database, const FString& ID,FString RoomName, FString ChattingMessage);
	static bool ChangeUserIntroduction(UMySQLConnection* Database, const FString& ID, FString& NewIntroduction);
	/** Lobby End*/

private:
	static FMySQLConnectoreQueryResult Query(UMySQLConnection* Database,const FString& Query);
	
	//���� ������ ���ɼ��� �ִ� ���� �����ִ� �Լ�
	static void RemovePossibilityOfQueryInjection(FString& Input);

	//������ ���� ������ ���� �Լ�
	static bool CheckThePossibilityOfQueryInjection(const FString& Input);
	static bool CheckThePossibilityOfQueryInjection(const FString& Input, FString& ContainedReservedWord);

};
