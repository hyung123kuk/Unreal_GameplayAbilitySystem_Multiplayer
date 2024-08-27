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

	//Drop ���� ������ ������ ���� ��
	UFUNCTION(BlueprintImplementableEvent)
	void FromItemTypeData(USlotWidgetController* SlotWidgetController,int index);

	//Drop ���� �� ��ų ������ ���� ��
	UFUNCTION(BlueprintImplementableEvent)
	void FromSkillTypeData(USlotWidgetController* SlotWidgetController, int index);

	//Drag ���� ������ ������ ���� ��
	UFUNCTION(BlueprintImplementableEvent)
	void ToItemTypeData(USlotWidgetController* SlotWidgetController, int index);

	//Drag ���� ��ų ������ ���� ��
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

	//�ش� ���Կ� ���Ե� �� �ִ� Ÿ��
	// ex ) �κ��丮 -> ������ , ���� -> ������, ��ų
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

	//FSlotInfoWidgetControllerParams�� OriginSlotWitdet�� ����
	//���� �־���� ��ġ�� �ش� Widget�� WidgetController�� �̴� �ٸ� SlotWidget�� ���� �Ű����� ����.
	// ex) �� ���Կ� ������ ������ �־ ���� �������� �ִ� ���� �κ��丮 ���� �����̴�.
	UPROPERTY(BlueprintReadOnly)
	TMap<FSlotStruct, TObjectPtr<USlotWidgetController>> OriginWidgetControllers;

	UPROPERTY(EditAnywhere, Category = "Tooltip")
	bool UseTooltip; //TODO;

	UPROPERTY(EditAnywhere, Category = "Tooltip")
	TSubclassOf<USlot> TooltipClass; //TODO;

	UPROPERTY()
	UHKAbilitySystemComponent* HKASC;

};
