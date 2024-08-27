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
class UDamageTextComponent;


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
	virtual void OnRep_Pawn() override;
	virtual void OnRep_PlayerState() override;
	virtual void OnPossess(APawn* InPawn) override;

public:

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bMiss, bool bCriticalHit);

	UFUNCTION(BlueprintCallable)
	FHitResult GetClickMouseHitResult() { return ClickMouseHitResult; }
	FHitResult GetNowMousePointer() { return CursorHit; }
	FHitResult GetLockOnHitResult() { return LockOnTargetHitResult; }
	void SettingUserInventory(FInGamePlayerInfo PlayerInfo);
	void SettingUserSkillInventory();
	UInventory* GetInventory() { return Inventory; }
	void MoveToDestination(FVector Destination); // SkillAbility에서 멀면 다가가는 용도
	void LockOnTarget(FHitResult TargetResult,float DistanceToOccurTag, FGameplayTag EventTag);
	UFUNCTION(BlueprintCallable)
	USkillInventory* GetSkillInventory();
	
	UHKAbilitySystemComponent* GetASC();

public:
	UPROPERTY(BlueprintAssignable)
	FQuickSlotDelegate QuickSlotPressedDelegate;

	UPROPERTY(BlueprintAssignable)
	FQuickSlotDelegate QuickSlotHeldDelegate;

	UPROPERTY(BlueprintAssignable)
	FQuickSlotDelegate QuickSlotReleasedDelegate;

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
	void InitHUD();
	void InitASCToWidget();
	bool bInitASCToWidget;
private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> HKContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MouseRightAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> CameraRotateAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MouseWheelAction;

	void MouseRightPressed() { bMouseRightDown = true; };
	void MouseRightReleased() { bMouseRightDown = false; };
	bool bMouseRightDown = false;

	void ShiftPressed() { bShiftKeyDown = true; };
	void ShiftReleased() { bShiftKeyDown = false; };
	bool bShiftKeyDown = false;

	void Move(const FInputActionValue& InputActionValue);
	void CameraRotate(const FInputActionValue& InputActionValue);
	void CameraZoomInZoomOut(const FInputActionValue& InputActionValue);
	void AutoRun();

	void CursorTrace();
	IMouseTargetActorInterface* LastActor;
	IMouseTargetActorInterface* ThisActor;
	FHitResult CursorHit;
	FHitResult ClickMouseHitResult;

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

	bool bLockOn = false;
	FHitResult LockOnTargetHitResult;
	float LockOnDistanceToOccurTag;
	FGameplayTag LockOnEventTag;

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

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

};
