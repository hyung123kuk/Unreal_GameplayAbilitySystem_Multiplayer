// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HKUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject> WidgetController;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();


};
