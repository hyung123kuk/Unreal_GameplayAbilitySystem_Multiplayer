// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HKDedicatedServerGameModeBase.generated.h"

class UMySQLConnection;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKDedicatedServerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay();
	virtual bool AttemptedToLogin(const FString& ID, const FString& Password, FString& ErrorMessage);
	
protected:
	/** Database */
	void ImportMySQLAccountInformationFromJson();
	/** Database End */


protected:

	/** Database */
	UPROPERTY()
	TObjectPtr<UMySQLConnection> UserData;

	FString DatabaseHost;
	FString DatabaseUserName;
	FString DatabaseUserPassword;
	FString DatabaseName;
	bool Init = false;
	/** Database End */

};
