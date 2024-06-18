// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Game/HKDedicatedServerGameModeBase.h"
#include "HKLoginGameMode.generated.h"

//�������� ����ϴ� GameMode�Դϴ�. 
//�� GameMode�� ����Ϸ��� Ŭ���̾�Ʈ�� Client�̰�,
//������ Dedicated Server���� �մϴ�.

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
