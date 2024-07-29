// Copyright Druid Mechanics


#include "UI/Widget/Slot.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SlotWidgetController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"

void USlot::Init(UHKSlotWidget* Parent, TArray<ESlotContainInformation> ContainTypes,int SlotNum, USlot* DSlot)
{
	ParentWidget = Parent;
	CanContainTypes = ContainTypes;
	DragSlot = DSlot;
	SlotNumber = SlotNum;
}

void USlot::SetSlotWIdgetController(USlotWidgetController* slotWidgetController)
{
	SlotWidgetController = slotWidgetController;
	Refresh();
}

ESlotContainInformation USlot::GetContainInformationType()
{
	return SlotWidgetController->GetContainInfo();
}

FReply USlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Left Button Down"));

		reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	}

	return reply.NativeReply;
}

void USlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (SlotWidgetController != nullptr && OutOperation == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Draging Start"));

		UDragDropOperation* oper = NewObject<UDragDropOperation>();
		OutOperation = oper;
		DragSlot->Init(ParentWidget, CanContainTypes, SlotNumber, nullptr);
		DragSlot->Refresh();
		oper->DefaultDragVisual = DragSlot;
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Draging Again"));
	}
}

bool USlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	
	if (InOperation != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Success"));
		USlot* OperationSlot = Cast<USlot>(InOperation->DefaultDragVisual);
		
		//받는 위젯에서 받을 수 있는 슬롯 아이템 타입인 경우
		ESlotContainInformation DragSlotContainType = OperationSlot->GetContainInformationType();
		if (CanContainTypes.Contains(DragSlotContainType) && ParentWidget->CanPutItemToSlot(DragSlotContainType))
		{
			ParentWidget->SwapSlot(OperationSlot->GetParentWidget(), OperationSlot->SlotNumber, ParentWidget, SlotNumber);
			return true;
		}
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Fali"));
	return false;
}
