// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/HKCharacterBase.h"
#include "HKCharacter.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKCharacter : public AHKCharacterBase
{
	GENERATED_BODY()
public:
	AHKCharacter();
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	virtual void InitAbilityActorInfo() override;

};
