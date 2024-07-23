// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AbilityInterface.generated.h"

class UHKAbilitySystemComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbilityInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UNREALPORTFOLIO_API IAbilityInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual bool GetLocalPlayerCondition(UHKAbilitySystemComponent* AbilitySystemComponent );
};
