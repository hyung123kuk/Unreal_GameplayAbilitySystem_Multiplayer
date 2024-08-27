// Copyright Druid Mechanics


#include "UI/Widget/Slot.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SlotWidgetController.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Player/HKPlayerController.h"
#include "Skill/SkillInventory.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"

void USlot::Init(UHKSlotWidget* Parent, TArray<ESlotContainInformation> ContainTypes,int SlotNum, USlot* DSlot)
{
	SlotWidget = Parent;
	CanContainTypes = ContainTypes;
	DragSlot = DSlot;
	SlotNumber = SlotNum;
}

void USlot::SetAbilitySystemComponents(UHKAbilitySystemComponent* ASC)
{
	K2_SetCooldown(FGameplayTag(), false, 0.f);
	ASC->SkillCooldownDelegate.AddDynamic(this, &USlot::SetCooldown);
}

void USlot::RemoveSlotWidgetController()
{
	SlotWidgetController = nullptr;
	Refresh();
}

void USlot::SetSlotWIdgetController(USlotWidgetController* slotWidgetController)
{
	SlotWidgetController = slotWidgetController;
	Refresh();
}

void USlot::PressedSlot()
{
	AHKPlayerController* PlayerController = Cast<AHKPlayerController>(GetOwningPlayer());
	if (GetContainInformationType() == ESlotContainInformation::Item)
	{
		FSlotInfoWidgetControllerParams SlotInfoParams = SlotWidgetController->GetSlotInfoParmas();
		PlayerController->GetInventory()->UseItem(SlotInfoParams.Id, SlotInfoParams.UniqueId);
	}
	else if (GetContainInformationType() == ESlotContainInformation::Skill)
	{
		FSlotInfoWidgetControllerParams SlotInfoParams = SlotWidgetController->GetSlotInfoParmas();
		PlayerController->GetSkillInventory()->PressedSkill(SlotInfoParams.Id);
	}
}

void USlot::HeldSlot()
{
	AHKPlayerController* PlayerController = Cast<AHKPlayerController>(GetOwningPlayer());
	if (GetContainInformationType() == ESlotContainInformation::Skill)
	{
		FSlotInfoWidgetControllerParams SlotInfoParams = SlotWidgetController->GetSlotInfoParmas();
		PlayerController->GetSkillInventory()->HeldSkill(SlotInfoParams.Id);
	}
}

void USlot::ReleaseSlot()
{
	AHKPlayerController* PlayerController = Cast<AHKPlayerController>(GetOwningPlayer());
	if (GetContainInformationType() == ESlotContainInformation::Skill)
	{
		FSlotInfoWidgetControllerParams SlotInfoParams = SlotWidgetController->GetSlotInfoParmas();
		PlayerController->GetSkillInventory()->ReleasedSkill(SlotInfoParams.Id);
	}
}

ESlotContainInformation USlot::GetContainInformationType()
{
	if (SlotWidgetController == nullptr)
		return ESlotContainInformation::Empty;

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
	else if (InMouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Click : Right Button Down"));
		PressedSlot();
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
		DragSlot->Init(SlotWidget, CanContainTypes, SlotNumber, nullptr);
		DragSlot->SetSlotWIdgetController(SlotWidgetController);
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
		
		ESlotContainInformation DragSlotContainType = OperationSlot->GetContainInformationType();
		if (CanContainTypes.Contains(DragSlotContainType) && SlotWidget->CanPutItemToSlot(DragSlotContainType))
		{
			SlotWidget->SwapSlot(OperationSlot->GetParentWidget(), OperationSlot->SlotNumber, SlotWidget, SlotNumber);
			return true;
		}
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Drag : Fali"));
	return false;
}

void USlot::SetCooldown(FGameplayTag CooldownTag, bool bCooldown, float RemainTime)
{
	if (SlotWidgetController == nullptr)
	{
		K2_SetCooldown(FGameplayTag(), false, 0.f);
		return;
	}

	if (SlotWidgetController->GetSlotInfoParmas().CooldownTag == FGameplayTag())
	{
		K2_SetCooldown(FGameplayTag(), false, 0.f);
		return;
	}

	if (CooldownTag == SlotWidgetController->GetSlotInfoParmas().CooldownTag)
	{
		K2_SetCooldown(CooldownTag, bCooldown, RemainTime);
	}
}
