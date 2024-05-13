// Copyright Druid Mechanics


#include "HKDatabaseFunctionLibrary.h"
#include "MySQLDatabase.h"
#include "MySQLConnection.h"
#include "UnrealPortfolio/UnrealPortfolio.h"

UMySQLConnection* UHKDatabaseFunctionLibrary::AttemptToConnectDataBase(const FString& Host, const FString& UserName, const FString& UserPassword, const FString& DatabaseName)
{
	UMySQLConnection* SQLConnection = UMySQLDatabase::MySQLInitConnection(Host, UserName, UserPassword, DatabaseName);
	if (IsValid(SQLConnection))
	{
		if (SQLConnection->MySQLCheckConnection())
		{
			return SQLConnection;
		}
	}

	checkf(false, TEXT("Host : %s UserName : %s UserPassword : %s DatabaseName : %s"), *Host, *UserName, *UserPassword, *DatabaseName);
	return nullptr;
}

bool UHKDatabaseFunctionLibrary::CreateUserID(UMySQLConnection* Database, const FString& ID, const FString& Password)
{
	if (!CheckThePossibilityOfQueryInjection(ID) || !CheckThePossibilityOfQueryInjection(Password))
	{
		return false;
	}

	FString QueryString = FString::Printf(TEXT("INSERT INTO `userdata`.`member` (`id`,`password`,`createTime`) VALUES('%s', sha2('%s', 224), NOW());"), *ID, *Password);
	FMySQLConnectoreQueryResult QueryResult = Query(Database, QueryString);

	return true;
}

bool UHKDatabaseFunctionLibrary::ExistedUserID(UMySQLConnection* Database,const FString& ID)
{
	if (!CheckThePossibilityOfQueryInjection(ID))
	{
		return false;
	}

	FString QueryString = FString::Printf(TEXT("SELECT id FROM member WHERE id = '%s'"), *ID);
	FMySQLConnectoreQueryResult QueryResult = Query(Database, QueryString);
	if (QueryResult.ResultRows.Num() == 0)
	{
		return false;
	}

	return true;
}

bool UHKDatabaseFunctionLibrary::MatchPasswordToID(UMySQLConnection* Database, const FString& ID, const FString& Password)
{
	if (!CheckThePossibilityOfQueryInjection(ID) || !CheckThePossibilityOfQueryInjection(Password))
	{
		return false;
	}

	FString QueryString = FString::Printf(TEXT("SELECT * FROM member WHERE id = '%s' AND PASSWORD = sha2('%s',224)"), *ID, *Password);
	FMySQLConnectoreQueryResult QueryResult = Query(Database, QueryString);
	if (QueryResult.ResultRows.Num() == 0)
	{
		return false;
	}

	return true;
}


FMySQLConnectoreQueryResult UHKDatabaseFunctionLibrary::Query(UMySQLConnection* Database,const FString& Query)
{
	FMySQLConnectoreQueryResult QueryResult = UMySQLDatabase::MySQLConnectorGetData(Query, Database);
	if (!QueryResult.Success)
	{
		UE_LOG(ServerLog, Warning, TEXT("쿼리 실패 : %s "), *Query);
		UE_LOG(ServerLog, Warning, TEXT("쿼리 실패 이유 메세지 : %s "), *QueryResult.ErrorMessage);
		return FMySQLConnectoreQueryResult();
	}

	return QueryResult;
}

bool UHKDatabaseFunctionLibrary::CheckThePossibilityOfQueryInjection(const FString& Input)
{
	FString ReservedWord;
	return CheckThePossibilityOfQueryInjection(Input, ReservedWord);
}

//특수문자 체크
//예약어 체크
bool UHKDatabaseFunctionLibrary::CheckThePossibilityOfQueryInjection(const FString& Input, FString& ContainedReservedWord)
{
	static const FString SpecialTexts[]
	{
		"%",
		"=",
		"*",
		">",
		">"
	};

	static const FString ReservedWords[]
	{
	  "OR",
	  "SELECT",
	  "INSERT",
	  "DELETE",
	  "UPDATE",
	  "CREATE",
	  "DROP",
	  "EXEC",
	  "UNION",
	  "FETCH",
	  "DECLARE",
	  "TRUNCATE"
	};

	for (FString Word : SpecialTexts)
	{
		if (Input.Contains(Word))
		{
			ContainedReservedWord = Word;
			return false;
		}
	}

	for (FString Word : ReservedWords)
	{
		if (Input.Contains(Word,ESearchCase::IgnoreCase))
		{
			ContainedReservedWord = Word;
			return false;
		}
	}

	return true;
}





