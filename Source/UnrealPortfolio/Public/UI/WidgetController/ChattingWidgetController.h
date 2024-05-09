// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKWidgetControllerBase.h"
#include "Interaction/UIPageInterface.h"
#include "ChattingWidgetController.generated.h"

USTRUCT(BlueprintType)
struct FChattingWidgetControllerParams
{
	GENERATED_BODY()

	FChattingWidgetControllerParams() {}
	FChattingWidgetControllerParams(FString& chat) :
		Chat(chat) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Chat;

};


UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API UChattingWidgetController : public UHKWidgetControllerBase, public IUIPageInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FChattingWidgetControllerParams& Params);

	virtual FString GetElementName() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetController")
	FString Chat;

};
