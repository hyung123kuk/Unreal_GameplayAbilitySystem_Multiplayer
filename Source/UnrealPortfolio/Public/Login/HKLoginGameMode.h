// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Game/HKDedicatedServerGameModeBase.h"
#include "HKLoginGameMode.generated.h"

//서버에서 사용하는 GameMode입니다. 
//이 GameMode를 사용하려면 클라이언트는 Client이고,
//서버는 Dedicated Server여야 합니다.

UCLASS()
class UNREALPORTFOLIO_API AHKLoginGameMode : public AHKDedicatedServerGameModeBase
{
	GENERATED_BODY()
	
public:

	/** Sign Up Dedicated Server*/
	bool CheckIDForSignUp(const FString& ID, FString& ErrorMessage);

	bool CheckValidForSignUp(const FString& ID, const FString& Password, const FString& PasswordConfirm ,FString& ErrorMessage);
	/** Sign Up Dedicated Server End */
};
