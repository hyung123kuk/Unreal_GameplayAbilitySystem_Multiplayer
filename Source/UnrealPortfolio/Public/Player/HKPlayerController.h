// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "Item/Inventory.h"
#include "Skill/SkillInfoData.h"
#include "HKPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class IMouseTargetActorInterface;
class USplineComponent;
class UHKAbilitySystemComponent;
class UHKInputConfig;
class UInventory;
struct FInGamePlayerInfo;
class UHKSlotWidget;
class USkillInventory;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuickSlotDelegate,const FGameplayTag& , InputSlotTag);

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	AHKPlayerController();
	virtual void PlayerTick(float DeltaTime) override;
public:
	UFUNCTION(BlueprintCallable)
	AActor* GetLastTargetActor() { return ClickMouseTarget; }
	virtual void OnPossess(APawn* aPawn) override;
	void SettingUserInformation(FInGamePlayerInfo PlayerInfo);
	UInventory* GetInventory() { return Inventory; }

	UFUNCTION(BlueprintCallable)
	USkillInventory* GetSkillInventory();

	UHKAbilitySystemComponent* GetASC();

public:
	UPROPERTY(BlueprintAssignable)
	FQuickSlotDelegate QuickSlotDelegate;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGameplayTag> QuickSlotTags;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UInventory> Inventory;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UHKSlotWidget> InventoryWidget;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UHKSlotWidget> SkillWindowWidget;

	UFUNCTION()
	void TryUseItem(FUserItem Item);

	UPROPERTY(BlueprintReadOnly)
	FUserItem TempUseItem; // 방금 사용한 아이템

	void ToggleInventory();
	void ToggleSkillWindow();

	UPROPERTY(EditAnywhere)
	TArray<FUserItem> StartItems;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnRep_PlayerState() override;
	void InitHUD();

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> HKContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;

	void Move(const FInputActionValue& InputActionValue);
	void AutoRun();

	void CursorTrace();
	IMouseTargetActorInterface* LastActor;
	IMouseTargetActorInterface* ThisActor;
	FHitResult CursorHit;
	AActor* ClickMouseTarget;

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UHKInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UHKAbilitySystemComponent> HKAbilitySystemComponent;



	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UInventory> InventoryClass;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UHKSlotWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Inventory")
	TSubclassOf<UHKSlotWidget> SkillWindowWidgetClass;

};
