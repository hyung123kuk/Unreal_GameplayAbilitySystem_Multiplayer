// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "UI/HKUserWidget.h"
#include "GameplayTagContainer.h"
#include "HKSlotWidget.generated.h"


class USlot;
class UUniformGridPanel;
class UHKWidgetControllerBase;
class UHKAbilitySystemComponent;

UENUM(BlueprintType)
enum class ESlotContainInformation : uint8
{
	Item,
	Skill,
	Empty,
	None,
};


USTRUCT(BlueprintType)
struct FSlotStruct
{
	GENERATED_BODY()
	
	bool operator==(const FSlotStruct& Other) const
	{
		return (SlotInformation == Other.SlotInformation) && (Other.Id == Id) && (Other.UniqueId == UniqueId);
	}

	friend FORCEINLINE uint32 GetTypeHash(const FSlotStruct& SlotStructData)
	{
		return GetTypeHash(SlotStructData.Name);
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Explanation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESlotContainInformation SlotInformation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UTexture2D> SlotTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Id = -1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int UniqueId;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int Count;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag;
};

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
	bool GetOnlyDragSlot() { return OnlyDrag; }

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
	
	UFUNCTION(BlueprintCallable)
	void SetAbilitySystemComponent(UHKAbilitySystemComponent* ASC);

protected:
	virtual void NativePreConstruct() override;

	UFUNCTION(BlueprintCallable)
	void SetItemInfoToEmptySlot(USlotWidgetController* SlotWidgetController);

	UFUNCTION(BlueprintCallable)
	void SetItemInfoWithIndex(USlotWidgetController* SlotWidgetController, int index);

	UFUNCTION(BlueprintCallable)
	void AddNewWidgetController(FSlotStruct NewSlotItem);

	UFUNCTION(BlueprintCallable)
	void RemoveWidgetController(FSlotStruct SlotItem);

	UFUNCTION(BlueprintCallable)
	void ChangeValueWidgetController(FSlotStruct SlotItem);

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

	UPROPERTY(EditAnywhere, Category = "Slot")
	bool OnlyDrag = false;

	//FSlotInfoWidgetControllerParams의 OriginSlotWitdet을 참고
	//원래 있어야할 위치가 해당 Widget인 WidgetController들 이는 다른 SlotWidget에 가도 옮겨지지 않음.
	// ex) 퀵 슬롯에 아이템 정보를 넣어도 원래 아이템이 있던 곳은 인벤토리 위젯 정보이다.
	UPROPERTY(BlueprintReadOnly)
	TMap<FSlotStruct, TObjectPtr<USlotWidgetController>> OriginWidgetControllers;

	UPROPERTY(EditAnywhere, Category = "Tooltip")
	bool UseTooltip; //TODO;

	UPROPERTY(EditAnywhere, Category = "Tooltip")
	TSubclassOf<USlot> TooltipClass; //TODO;

	UPROPERTY()
	UHKAbilitySystemComponent* HKASC;

};
