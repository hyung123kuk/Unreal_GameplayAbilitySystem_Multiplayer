// Copyright Druid Mechanics


#include "UI/Widget/HKPageWidget.h"
#include "Components/PanelWidget.h"
#include "Blueprint/UserWidget.h"
#include "Interaction/UIPageInterface.h"
#include "UI/HKWidgetControllerBase.h"


void UHKPageWidget::AddElement(UHKWidgetControllerBase* NewElement)
{
	IUIPageInterface* NewPageInterface = Cast<IUIPageInterface>(NewElement);
	for (UHKWidgetControllerBase* Element : AllElements)
	{
		IUIPageInterface* ElementInterface = Cast<IUIPageInterface>(Element);
		if (NewPageInterface->GetElementName() == ElementInterface->GetElementName())
		{
			return;
		}
	}

	AllElements.Add(NewElement);
	RefreshCurrentPage();
}

void UHKPageWidget::RemoveElement(const FString& ParamName)
{
	for (UHKWidgetControllerBase* Element : AllElements)
	{
		IUIPageInterface* ElementInterface = Cast<IUIPageInterface>(Element);

		if (ElementInterface->GetElementName() == ParamName)
		{
			AllElements.Remove(Element);
			RefreshCurrentPage();
			return;
		}
	}
}

bool UHKPageWidget::DoesCurrentPageExist()
{
	return DoesPageExist(NowPage);
}

bool UHKPageWidget::DoesPrevPageExist()
{
	return DoesPageExist(NowPage - 1);
}

bool UHKPageWidget::DoesNextPageExist()
{
	return DoesPageExist(NowPage + 1);
}

void UHKPageWidget::RefreshCurrentPage()
{
	TArray<TObjectPtr<UHKWidgetControllerBase>> NowPages = GetPageElements(NowPage);
	
	ElementsOfCurrentPage = NowPages;
	Refresh();
}

void UHKPageWidget::TurnToPrevPage()
{
	if (!DoesPrevPageExist())
		return;

	TArray<TObjectPtr<UHKWidgetControllerBase>> PrevPages = GetPageElements(NowPage - 1);
	if (PrevPages.Num() == 0)
		return;

	ElementsOfCurrentPage = PrevPages;
	NowPage--;
	Refresh();
}

void UHKPageWidget::TurnToNextPage()
{
	if (!DoesNextPageExist())
		return;

	TArray<TObjectPtr<UHKWidgetControllerBase>> NextPages = GetPageElements(NowPage + 1);
	if (NextPages.Num() == 0)
		return;

	ElementsOfCurrentPage = NextPages;
	NowPage++;
	Refresh();
}

bool UHKPageWidget::DoesPageExist(int Page)
{
	if (Page < 0)
		return false;

	int TotalPageIndex = GetTotalPages() - 1;

	if (Page > TotalPageIndex)
		return false;

	return true;
}

TArray<TObjectPtr<UHKWidgetControllerBase>> UHKPageWidget::GetPageElements(int Page)
{
	if (!DoesPageExist(Page))
		return TArray<TObjectPtr<UHKWidgetControllerBase>>();

	int CurrentPageStartElementIndex = CountPerPage * Page;
	int NextPageStartElementIndex = CountPerPage * (Page + 1);

	int CurrentPageLastElementIndex = NextPageStartElementIndex - 1;
	int TotalElementCount = AllElements.Num();
	if (CurrentPageLastElementIndex > TotalElementCount - 1)
	{
		CurrentPageLastElementIndex = TotalElementCount - 1;
	}

	TArray<TObjectPtr<UHKWidgetControllerBase>> PageElements;
	for(int i = CurrentPageStartElementIndex; i<= CurrentPageLastElementIndex; i++)
	{
		PageElements.Add(AllElements[i]);
	}

	return PageElements;
}

int UHKPageWidget::GetTotalPages()
{
	int TotalPages = AllElements.Num() / CountPerPage;
	if (AllElements.Num() % CountPerPage > 0)
	{
		TotalPages++;
	}

	return TotalPages;
}


