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

	
protected:
	/** Database */
	void ImportMySQLAccountInformationFromJson();
	bool AttemptedToLogin(const FString& Id, const FString& Password, FString& ErrorMessage);
	/** Database End */


protected:

	/** Database */
	UPROPERTY()
	TObjectPtr<UMySQLConnection> UserData;
	/** Database End */

};
