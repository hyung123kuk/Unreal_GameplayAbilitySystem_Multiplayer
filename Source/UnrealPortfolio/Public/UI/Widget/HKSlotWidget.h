// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKUserWidget.h"
#include "HKSlotWidget.generated.h"


class USlot;
class UUniformGridPanel;

UENUM(BlueprintType)
enum class ESlotContainInformation : uint8
{
	Item,
	Skill,
	None
};

class UHKWidgetControllerBase;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKSlotWidget : public UHKUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool CanPutItemToSlot(ESlotContainInformation SlotInformationType);

	int FindEmptySlotIndex();
	void SwapSlot(UHKSlotWidget* DragSlotWidget, int DragIndex, UHKSlotWidget* DropSlotWidget, int DropIndex);
	TArray<TObjectPtr<USlot>> GetSlots() { return Slots; }


	//Drop 슬롯 아이템 정보가 왔을 때
	UFUNCTION(BlueprintImplementableEvent)
	void FromItemTypeData(USlotWidgetController* SlotWidgetController,int index);

	//Drop 슬롯 에 스킬 정보가 왔을 때
	UFUNCTION(BlueprintImplementableEvent)
	void FromSkillTypeData(USlotWidgetController* SlotWidgetController, int index);

	//Drag 슬롯 아이템 정보가 왔을 때
	UFUNCTION(BlueprintImplementableEvent)
	void ToItemTypeData(USlotWidgetController* SlotWidgetController, int index);

	//Drag 슬롯 스킬 정보가 왔을 때
	UFUNCTION(BlueprintImplementableEvent)
	void ToSkillTypeData(USlotWidgetController* SlotWidgetController, int index);
	

protected:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetItemInfoToEmptySlot(USlotWidgetController* SlotWidgetController);

	UFUNCTION(BlueprintCallable)
	void SetItemInfoWithIndex(USlotWidgetController* SlotWidgetController, int index);


protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> Root;

	UPROPERTY(EditAnywhere, Category = "Slot")
	int SlotRow;

	UPROPERTY(EditAnywhere, Category = "Slot")
	int SlotColumn;

	//해당 슬롯에 포함될 수 있는 타입
	// ex ) 인벤토리 -> 아이템 , 슬롯 -> 아이템, 스킬
	UPROPERTY(EditAnywhere, Category = "Slot")
	TArray<ESlotContainInformation> CanContainTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slot")
	TSubclassOf<USlot> SlotClass;
	
	UPROPERTY(EditAnywhere, Category = "Slot")
	TSubclassOf<USlot> DragSlotClass;
	
	UPROPERTY()
	TObjectPtr<USlot> DragSlot;

	UPROPERTY(BlueprintReadOnly)
	TArray<TObjectPtr<USlot>> Slots;

	UPROPERTY(EditAnywhere, Category = "Tooltip")
	bool UseTooltip; //TODO;

	UPROPERTY(EditAnywhere, Category = "Tooltip")
	TSubclassOf<USlot> TooltipClass; //TODO;

	
};
