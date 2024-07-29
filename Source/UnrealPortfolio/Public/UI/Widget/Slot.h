// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKUserWidget.h"
#include "UI/Widget/HKSlotWidget.h"
#include "Slot.generated.h"

class UHKWidgetControllerBase;
class USlotWidgetController;
/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API USlot : public UHKUserWidget
{
	GENERATED_BODY()
	
public:
	void Init(UHKSlotWidget* Parent, TArray<ESlotContainInformation> ContainTypes, int SlotNum, USlot* DSlot);

	UFUNCTION(BlueprintImplementableEvent)
	void Refresh();

	void SetSlotWIdgetController(USlotWidgetController* slotWidgetController);

	ESlotContainInformation GetContainInformationType();
	UHKSlotWidget* GetParentWidget() { return ParentWidget; }
	USlotWidgetController* GetSlotWidgetController() { return SlotWidgetController; }

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USlotWidgetController* SlotWidgetController;

private:
	TArray<ESlotContainInformation> CanContainTypes;

	UPROPERTY()
	TObjectPtr<USlot> DragSlot;

	UPROPERTY()
	TObjectPtr<UHKSlotWidget> ParentWidget;

	int SlotNumber;

};
