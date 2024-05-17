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
	/// DataBase는 DedicatedServer에서만 접근할 수 있으므로 DedicatedServer를 상속받은 GameMode에서만 접근 가능합니다.
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
	
	//쿼리 인젝션 가능성이 있는 문자 지워주는 함수
	static void RemovePossibilityOfQueryInjection(FString& Input);

	//간단한 쿼리 인젝션 예방 함수
	static bool CheckThePossibilityOfQueryInjection(const FString& Input);
	static bool CheckThePossibilityOfQueryInjection(const FString& Input, FString& ContainedReservedWord);

};
