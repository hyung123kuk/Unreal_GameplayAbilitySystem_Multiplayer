// Copyright Druid Mechanics


#include "UI/Widget/HKSlotWidget.h"
#include "UI/Widget/Slot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "UI/HKWidgetControllerBase.h"
#include "UI/SlotWidgetController.h"

void UHKSlotWidget::SetAbilitySystemComponent(UHKAbilitySystemComponent* ASC)
{
	HKASC = ASC;
	for (USlot* slot : Slots)
	{
		slot->SetAbilitySystemComponents(ASC);
	}
}

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
		FSlotInfoWidgetControllerParams SlotWidget = SlotWidgetController->GetSlotInfoParmas();
		if (SlotWidget.OriginSlotWitdet == nullptr)
		{
			SlotWidget.OriginSlotWitdet = this;
		}

		SlotWidget.SlotWitdet = this;
		SlotWidget.SlotNumber = EmptySlotindex;
		SlotWidgetController->SetWidgetControllerParams(SlotWidget);
		SetItemInfoWithIndex(SlotWidgetController, EmptySlotindex);
	}
}

void UHKSlotWidget::SetItemInfoWithIndex(USlotWidgetController* SlotWidgetController, int index)
{
	if (index >= Slots.Num())
	{
		return;
	}

	if (SlotWidgetController == nullptr)
	{
		Slots[index]->RemoveSlotWidgetController();
		return;
	}

	FSlotInfoWidgetControllerParams SlotWidget = SlotWidgetController->GetSlotInfoParmas();
	SlotWidget.SlotWitdet = this;
	SlotWidget.SlotNumber = index;
	SlotWidgetController->SetWidgetControllerParams(SlotWidget);

	Slots[index]->SetSlotWIdgetController(SlotWidgetController);
}

void UHKSlotWidget::AddNewWidgetController(FSlotStruct NewSlotItem)
{
	USlotWidgetController* NewSlotWidgetController = NewObject<USlotWidgetController>(this);
	FSlotInfoWidgetControllerParams SlotWidgetControllerParams = NewSlotWidgetController->GetSlotInfoParmas();
	SlotWidgetControllerParams.SlotImage = NewSlotItem.SlotTexture;
	SlotWidgetControllerParams.Count = NewSlotItem.Count;
	SlotWidgetControllerParams.ContainInfo = NewSlotItem.SlotInformation;
	SlotWidgetControllerParams.Id = NewSlotItem.Id;
	SlotWidgetControllerParams.UniqueId= NewSlotItem.UniqueId;
	SlotWidgetControllerParams.CooldownTag = NewSlotItem.CooldownTag;
	NewSlotWidgetController->SetWidgetControllerParams(SlotWidgetControllerParams);
	OriginWidgetControllers.Add(NewSlotItem, NewSlotWidgetController);
	SetItemInfoToEmptySlot(NewSlotWidgetController);
}

void UHKSlotWidget::RemoveWidgetController(FSlotStruct SlotItem)
{
	for (auto OriginWidgetController : OriginWidgetControllers)
	{
		USlotWidgetController* SlotWidgetController = OriginWidgetController.Value;
		if (SlotWidgetController->GetSlotInfoParmas().Id == SlotItem.Id)
		{
			FSlotInfoWidgetControllerParams SlotInfoParmas = SlotWidgetController->GetSlotInfoParmas();
			USlot* RemoveSlot = SlotInfoParmas.SlotWitdet->GetSlots()[SlotInfoParmas.SlotNumber];
			RemoveSlot->RemoveSlotWidgetController();
		}
	}
}

void UHKSlotWidget::ChangeValueWidgetController(FSlotStruct SlotItem)
{
	for (auto OriginWidgetController : OriginWidgetControllers)
	{
		USlotWidgetController* SlotWidgetController = OriginWidgetController.Value;
		if (SlotWidgetController->GetSlotInfoParmas().Id == SlotItem.Id)
		{
			FSlotInfoWidgetControllerParams SlotInfoParams = SlotWidgetController->GetSlotInfoParmas();
			SlotInfoParams.Count = SlotItem.Count;
			SlotWidgetController->SetWidgetControllerParams(SlotInfoParams);
			USlot* ChangeSlot = SlotInfoParams.SlotWitdet->GetSlots()[SlotInfoParams.SlotNumber];
			ChangeSlot->Refresh();
			break;
		}
	}
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

	//끝 슬롯은 시작 슬롯의 아이템 타입에 따라 다른 함수 호출
	switch (DragSlotItemType)
	{
	case ESlotContainInformation::Item:
		DropSlotWidget->FromItemTypeData(DragSlotWidgetController, DropIndex);
		break;
	case ESlotContainInformation::Skill:
		DropSlotWidget->FromSkillTypeData(DragSlotWidgetController, DropIndex);
		break;
	case ESlotContainInformation::Empty:
		DropSlotWidget->GetSlots()[DropIndex]->RemoveSlotWidgetController();
		break;
	default:
		check(false);
		break;
	}

	//시작 슬롯은 끝 슬롯의 아이템 타입에 따라 다른 함수 호출
	switch (DropSlotItemType)
	{
		case ESlotContainInformation::Item:
			DragSlotWidget->ToItemTypeData(DropSlotWidgetController, DragIndex);
			break;
		case ESlotContainInformation::Skill:
			DragSlotWidget->ToSkillTypeData(DropSlotWidgetController, DragIndex);
			break;
		case ESlotContainInformation::Empty:
			if (!DragSlotWidget->GetOnlyDragSlot()) // 끝점이 빈 슬롯이면 비울 것인지 (스킬창에서는 옮겼다고 비우면 안됨)
				DragSlotWidget->GetSlots()[DragIndex]->RemoveSlotWidgetController();
			break;

		default:
			check(false);
			break;
	}

	return;
}


