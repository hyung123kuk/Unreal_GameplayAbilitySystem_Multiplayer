// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HKGameMode.generated.h"

class UCharacterClassInfo;
/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	UPROPERTY(EditDefaultsOnly, Category = "CharacterClass Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

};
