// Copyright Druid Mechanics


#include "UI/Widget/HKSlotWidget.h"
#include "UI/Widget/Slot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "UI/HKWidgetControllerBase.h"

void UHKSlotWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (DragSlotClass != nullptr)
	{
		DragSlot = CreateWidget<USlot>(GetWorld(), DragSlotClass);
		
	}

	for (int i = 0; i < SlotRow; i++)
	{
		for (int j = 0; j < SlotColumn; j++)
		{
			USlot* NewSlot = CreateWidget<USlot>(GetWorld(), SlotClass);
			UUniformGridSlot* UniformSlot = Root->AddChildToUniformGrid(NewSlot, i, j);
			UniformSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			UniformSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
			NewSlot->Init(this, CanContainTypes, Slots.Num(), DragSlot);
			Slots.Add(NewSlot);
		}
	}
}

void UHKSlotWidget::SetItemInfoToEmptySlot(USlotWidgetController* SlotWidgetController)
{
	int EmptySlotindex = FindEmptySlotIndex();
	if (EmptySlotindex != -1)
	{
		SetItemInfoWithIndex(SlotWidgetController, EmptySlotindex);
	}
}

void UHKSlotWidget::SetItemInfoWithIndex(USlotWidgetController* SlotWidgetController, int index)
{
	if (index < Slots.Num())
	{
		return;
	}

	Slots[index]->SetSlotWIdgetController(SlotWidgetController);
}

bool UHKSlotWidget::CanPutItemToSlot(ESlotContainInformation SlotInformationType)
{
	if (FindEmptySlotIndex() == -1 && !CanContainTypes.Contains(SlotInformationType))
	{
		return false;
	}

	return true;
}

int UHKSlotWidget::FindEmptySlotIndex()
{
	for (int i = 0; i < Slots.Num(); i++)
	{
		if (Slots[i]->GetSlotWidgetController() == nullptr)
			return i;
	}

	return -1;
}

void UHKSlotWidget::SwapSlot(UHKSlotWidget* DragSlotWidget, int DragIndex, UHKSlotWidget* DropSlotWidget, int DropIndex)
{
	ESlotContainInformation DragSlotItemType = DragSlotWidget->GetSlots()[DragIndex]->GetContainInformationType();
	ESlotContainInformation DropSlotItemType = DropSlotWidget->GetSlots()[DropIndex]->GetContainInformationType();
	USlotWidgetController* DragSlotWidgetController = DragSlotWidget->GetSlots()[DragIndex]->GetSlotWidgetController();
	USlotWidgetController* DropSlotWidgetController = DropSlotWidget->GetSlots()[DropIndex]->GetSlotWidgetController();

	//시작 슬롯은 끝 슬롯의 아이템 타입에 따라 다른 함수 호출
	switch (DropSlotItemType)
	{
		case ESlotContainInformation::Item:
			DragSlotWidget->ToItemTypeData(DropSlotWidgetController, DragIndex);
			break;
		case ESlotContainInformation::Skill:
			DragSlotWidget->ToSkillTypeData(DropSlotWidgetController, DragIndex);
			break;

		default:
			check(false);
			break;
	}

	//끝 슬롯은 시작 슬롯의 아이템 타입에 따라 다른 함수 호출
	switch (DragSlotItemType)
	{
		case ESlotContainInformation::Item:
			DropSlotWidget->FromItemTypeData(DragSlotWidgetController, DropIndex);
			break;
		case ESlotContainInformation::Skill:
			DropSlotWidget->FromSkillTypeData(DragSlotWidgetController, DropIndex);
			break;

		default:
			check(false);
			break;
	}

	return;
}


