// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKUserWidget.h"
#include "HKPageWidget.generated.h"

class UHKWidgetControllerBase;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKPageWidget : public UHKUserWidget
{
	GENERATED_BODY()


public:
	UFUNCTION(BlueprintImplementableEvent)
	void Refresh();

	UFUNCTION(BlueprintCallable)
	void AddElement(UHKWidgetControllerBase* NewElement);

	UFUNCTION(BlueprintCallable)
	void RemoveElement(const FString& ParamName);

	UFUNCTION(BlueprintCallable)
	bool DoesCurrentPageExist();

	UFUNCTION(BlueprintCallable)
	bool DoesPrevPageExist();

	UFUNCTION(BlueprintCallable)
	bool DoesNextPageExist();
	
	UFUNCTION(BlueprintCallable)
	void RefreshCurrentPage();

	UFUNCTION(BlueprintCallable)
	void TurnToPrevPage();

	UFUNCTION(BlueprintCallable)
	void TurnToNextPage();

private:
	bool DoesPageExist(int Page);
	TArray<TObjectPtr<UHKWidgetControllerBase>> GetPageElements(int Page);
	int GetTotalPages();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InitSettings")
	int CountPerPage;

protected:
	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UHKWidgetControllerBase>> ElementsOfCurrentPage;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<UHKWidgetControllerBase>> AllElements;

	UPROPERTY(BlueprintReadOnly)
	int NowPage;

};
