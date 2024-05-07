// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/HKCharacterBase.h"
#include "Interaction/CombatInterface.h"
#include "HKEnemy.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKEnemy : public AHKCharacterBase
{
	GENERATED_BODY()
	
public:
	AHKEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
	/** Combat Interface */
	virtual void Die() override;
	/**Combat Interface End*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;
};
