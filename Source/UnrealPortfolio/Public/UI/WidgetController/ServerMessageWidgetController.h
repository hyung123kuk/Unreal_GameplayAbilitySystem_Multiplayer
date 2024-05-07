// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/HKWidgetControllerBase.h"
#include "ServerMessageWidgetController.generated.h"


USTRUCT(BlueprintType)
struct FServerMessageWidgetControllerParams
{
	GENERATED_BODY()
	FServerMessageWidgetControllerParams() {}
	FServerMessageWidgetControllerParams(FString msg): Message(msg) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Message;
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API UServerMessageWidgetController : public UHKWidgetControllerBase
{
	GENERATED_BODY()
public:
	
	void SetWidgetControllerParams(const FServerMessageWidgetControllerParams& WCParams);


protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	FString Message;
};

