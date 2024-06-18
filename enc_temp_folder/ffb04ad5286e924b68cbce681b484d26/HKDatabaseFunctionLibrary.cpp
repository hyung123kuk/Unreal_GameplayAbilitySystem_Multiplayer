// Copyright Druid Mechanics


#include "HKDatabaseFunctionLibrary.h"
#include "MySQLDatabase.h"
#include "MySQLConnection.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "Lobby/Store.h"
#include "Lobby/HKLobbyGameMode.h"

bool UHKDatabaseFunctionLibrary::NotifyOpenServer(UMySQLConnection* Database, const FString& ServerIP, FString& ServerNameRef)
{
	FString QueryString = FString::Printf(TEXT("CALL OPEN_SERVER('%s');"), *ServerIP);
	FMySQLConnectoreQueryResult QueryResult = Query(Database, QueryString);
	if (QueryResult.ResultRows.Num() > 0)
	{
		FMySQLConnectorQueryResultRow UserData = QueryResult.ResultRows[0];
		if (UserData.Fields.Num() == 0)
		{
			return false;
		}

		if (UserData.Fields.Num() > 0)
		{
			ServerNameRef = UserData.Fields[0].Value;
		}

		UE_LOG(ServerLog, Warning, TEXT("서버(%s) 오픈! "), *ServerNameRef);
		return true;
	}

	return false;
}

bool UHKDatabaseFunctionLibrary::NotifyUserCount(UMySQLConnection* Database, const FString& ServerIP, int CrowedLevel)
{
	FString QueryString = FString::Printf(TEXT("UPDATE `userdata`.`server_list` SET `crowded_level` = %d WHERE `ip` = '%s';"), CrowedLevel, *ServerIP);
	FMySQLConnectoreQueryResult QueryResult = Query(Database, QueryString, true);
	return true;
}

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

bool UHKDatabaseFunctionLibrary::GetStoreItemsInformation(UMySQLConnection* Database, TArray<FStoreItemDefaultInfo>& Items)
{
	FString QueryString = FString::Printf(TEXT("SELECT * FROM `userdata`.`store_item` WHERE `is_sale` = 1"));
	FMySQLConnectoreQueryResult QueryResult = Query(Database, QueryString);
	if (QueryResult.ResultRows.Num() > 0)
	{
		for (int i = 0; i < QueryResult.ResultRows.Num(); i++)
		{

			FMySQLConnectorQueryResultRow ItemData = QueryResult.ResultRows[i];
			int StoreItemId = 0;
			FString StoreItemName;
			int StoreItemCost=  0;
			FString StoreItemTexture;
			FString StoreItemExplanation;

			if (ItemData.Fields.Num() > 0)
			{
				StoreItemId = FCString::Atoi(*ItemData.Fields[0].Value);
			}
			if (ItemData.Fields.Num() > 1)
			{
				StoreItemName = ItemData.Fields[1].Value;
			}
			if (ItemData.Fields.Num() > 2)
			{
				StoreItemCost = FCString::Atoi(*ItemData.Fields[2].Value);
			}
			if (ItemData.Fields.Num() > 3)
			{
				StoreItemTexture = ItemData.Fields[3].Value;
			}
			if (ItemData.Fields.Num() > 4)
			{
				StoreItemExplanation = ItemData.Fields[4].Value;
			}


			UE_LOG(ServerLog, Warning, TEXT("데이터 베이스에서 스토어 아이템(%s)을 가격(%d)으로 가져왔습니다. "), *StoreItemName, StoreItemCost);

			FStoreItemDefaultInfo ItemInfo (StoreItemId, StoreItemName, StoreItemCost, StoreItemTexture, StoreItemExplanation);
			Items.Add(ItemInfo);
		}
	}

	return true;
}

bool UHKDatabaseFunctionLibrary::PurchaseItem(UMySQLConnection* Database, const FString& ID, int ItemID, int& LeftGold)
{
	FString QueryString = FString::Printf(TEXT("CALL PURCHASE_ITEM('%s',%d);"), *ID, ItemID);
	FMySQLConnectoreQueryResult QueryResult = Query(Database, QueryString);
	int itemCount=0;
	if (QueryResult.ResultRows.Num() > 0)
	{
		FMySQLConnectorQueryResultRow UserData = QueryResult.ResultRows[0];
		if (UserData.Fields.Num() == 0)
		{
			return false;
		}

		if (UserData.Fields.Num() > 0)
		{
			itemCount = FCString::Atoi(*UserData.Fields[0].Value);
		}
		if (UserData.Fields.Num() > 1)
		{
			LeftGold = FCString::Atoi(*UserData.Fields[1].Value);
		}

		UE_LOG(ServerLog, Warning, TEXT("남은 골드(%d) 아이템 개수(%d)"), LeftGold, itemCount);
		return true;
	}

	return false;
}


bool UHKDatabaseFunctionLibrary::GetUserInformation(UMySQLConnection* Database, const FString& ID, FString& Introduction, int& Gold, int& Exp)
{
	FString QueryString = FString::Printf(TEXT("SELECT `introduction`,`gold`,`exp` FROM `userdata`.`member` WHERE `id` = '%s';"), *ID);
	FMySQLConnectoreQueryResult QueryResult = Query(Database, QueryString);

	if (QueryResult.ResultRows.Num() > 0)
	{
		FMySQLConnectorQueryResultRow UserData = QueryResult.ResultRows[0];
		if (UserData.Fields.Num() == 0)
		{
			return false;
		}

		if (UserData.Fields.Num() > 0)
		{
			Introduction = UserData.Fields[0].Value;
		}
		if (UserData.Fields.Num() > 1)
		{
			Gold = FCString::Atoi(*UserData.Fields[1].Value);
		}
		if (UserData.Fields.Num() > 2)
		{
			Exp = FCString::Atoi(*UserData.Fields[2].Value);
		}
		return true;
	}

	return false;
}

bool UHKDatabaseFunctionLibrary::GetUserItemsInformation(UMySQLConnection* Database, const FString& ID, TArray<int>& Ids, TArray<int>& Count)
{
	FString QueryString = FString::Printf(TEXT("SELECT item_id,item_count FROM member_item WHERE user_id = '%s';"), *ID);
	FMySQLConnectoreQueryResult QueryResult = Query(Database, QueryString);
	if (!Database->MySQLCheckConnection())
	{
		UE_LOG(ServerLog, Error, TEXT("First GET_USERALLITEMS After"));
	}
	if (QueryResult.ResultRows.Num() > 0)
	{
		for (FMySQLConnectorQueryResultRow Row : QueryResult.ResultRows)
		{
			if (Row.Fields.Num() < 2)
			{
				continue;
			}

			int ItemId = 0;
			int ItemCount = 0;

			ItemId = FCString::Atoi(*Row.Fields[0].Value);
			ItemCount = FCString::Atoi(*Row.Fields[1].Value);

			Ids.Add(ItemId);
			Count.Add(ItemCount);

			UE_LOG(ServerLog, Warning, TEXT("유저(%s)의 아이템(%d)을 개수(%d)"), *ID, ItemId, ItemCount);
		}

		return true;
	}

	return false;
}

bool UHKDatabaseFunctionLibrary::RecordChatInDatabase(UMySQLConnection* Database, const FString& ID, FString RoomName, FString ChattingMessage)
{
	if (!CheckThePossibilityOfQueryInjection(ID))
	{
		return false;
	}

	FString DBRecordRoomName;
	if (RoomName.IsEmpty())
	{
		DBRecordRoomName.Append(TEXT("Lobby"));
	}
	else
	{
		DBRecordRoomName.Append(TEXT("Room_"));
	}

	//DB저장 전 예약어는 빼고 넣음
	RemovePossibilityOfQueryInjection(ChattingMessage);
	RemovePossibilityOfQueryInjection(RoomName);
	DBRecordRoomName.Append(RoomName);

	FString QueryString = FString::Printf(TEXT("INSERT INTO `userdata`.`chatting_log` (`user_id`,`chat_message`,`place`,`time`) VALUES('%s', '%s', '%s', NOW());"), *ID, *ChattingMessage, *DBRecordRoomName);
	FMySQLConnectoreQueryResult QueryResult = Query(Database, QueryString);

	return true;
}

bool UHKDatabaseFunctionLibrary::ChangeUserIntroduction(UMySQLConnection* Database, const FString& ID, FString& Introduction)
{
	if (!CheckThePossibilityOfQueryInjection(ID))
	{
		return false;
	}

	//DB저장 전 예약어는 빼고 넣음
	RemovePossibilityOfQueryInjection(Introduction);
	FString QueryString = FString::Printf(TEXT("UPDATE `userdata`.`member` SET `introduction` = '%s' WHERE `id` = '%s';"), *Introduction, *ID);
	FMySQLConnectoreQueryResult QueryResult = Query(Database, QueryString);
	
	return true;
}

FMySQLConnectoreQueryResult UHKDatabaseFunctionLibrary::Query(UMySQLConnection* Database,const FString& Query, bool PassNullMessageLog)
{
	FMySQLConnectoreQueryResult QueryResult = UMySQLDatabase::MySQLConnectorGetData(Query, Database);
	if (!QueryResult.Success)
	{
		if (PassNullMessageLog && QueryResult.ErrorMessage == TEXT("Result is NULL!"))
		{
			return FMySQLConnectoreQueryResult();
		}
		UE_LOG(ServerLog, Warning, TEXT("쿼리 실패 : %s "), *Query);
		UE_LOG(ServerLog, Warning, TEXT("쿼리 실패 이유 메세지 : %s "), *QueryResult.ErrorMessage);
		return FMySQLConnectoreQueryResult();
	}

	//끊켜져있으면 재연결 ( 프로시져 호출시 끊김 현상 발생 )
	if (!Database->MySQLCheckConnection())
	{
		Database->GameModeBase->ImportMySQLAccountInformationFromJson();
	}

	return QueryResult;
}

void UHKDatabaseFunctionLibrary::RemovePossibilityOfQueryInjection(FString& Input)
{
	FString ContainedReservedWord;
	while (!CheckThePossibilityOfQueryInjection(Input, ContainedReservedWord))
	{
		Input = Input.Replace(*ContainedReservedWord, TEXT(""), ESearchCase::IgnoreCase);
	}
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





