// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
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
struct FUserItem;

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

	void SettingUserInformation(FInGamePlayerInfo PlayerInfo);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void OnRep_PlayerState() override;
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

	UHKAbilitySystemComponent* GetASC();

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

	UPROPERTY()
	TObjectPtr<UInventory> Inventory;

	UPROPERTY(EditAnywhere)
	TArray<FUserItem> StartItems;

};
