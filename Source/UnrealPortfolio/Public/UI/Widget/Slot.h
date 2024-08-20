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
	void RemoveSlotWidgetController();
	void SetSlotWIdgetController(USlotWidgetController* slotWidgetController);

	UFUNCTION(BlueprintCallable)
	void PressedSlot();

	UFUNCTION(BlueprintCallable)
	void HeldSlot();

	UFUNCTION(BlueprintCallable)
	void ReleaseSlot();

	ESlotContainInformation GetContainInformationType();
	UHKSlotWidget* GetParentWidget() { return SlotWidget; }
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
	TObjectPtr<UHKSlotWidget> SlotWidget;

	int SlotNumber;

};
