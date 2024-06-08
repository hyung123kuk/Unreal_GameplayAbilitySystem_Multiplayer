// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKWidgetControllerBase.h"
#include "Lobby/Store.h"
#include "StoreWidgetController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeStoreValueSignature, UStoreWidgetController*, StoreInfoController);


UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API UStoreWidgetController : public UHKWidgetControllerBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const TArray<FStoreItemDefaultInfo>& Params);
	
	UPROPERTY(BlueprintAssignable, Category = "ChangeValue")
	FChangeStoreValueSignature ChangeStoreValueSignature;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	TArray<FStoreItemDefaultInfo> StoreItems;
};
