// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HKWidgetControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKWidgetControllerBase : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues() {};
	virtual void BindCallbacksToDependencies() {};
};
