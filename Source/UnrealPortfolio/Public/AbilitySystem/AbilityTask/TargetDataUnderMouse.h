// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetDataUnderMouse.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FMouseTargetDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle, const FGameplayTag&, ActivationTag);

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UTargetDataUnderMouse : public UAbilityTask
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks", meta = (DisplayName = "TargetDataUnderMouse", HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnlt = "true"))
	static UTargetDataUnderMouse* CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility, const TArray<AActor*> TargetActors);

	UFUNCTION()
	void SetCurrentPredictionKey(int16 NewPredictionKeyCurrent);

	UFUNCTION()
	void SetActivationTag(const FGameplayTag& GameplayTag);

	UFUNCTION()
	void SetMouseTarget(FHitResult MouseTarget);
	
	UPROPERTY(BlueprintAssignable)
	FMouseTargetDataSignature ValidData;

	TArray<TWeakObjectPtr<AActor>> TargetActor;



private:
	UPROPERTY()
	FHitResult LastMouseTargetHitResult;

	UPROPERTY()
	int16 PredictionKeyCurrent;

	UPROPERTY()
	FGameplayTag ActivationTag;

	virtual void Activate() override;
	void SendMouseCursorData();

	void OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag);
};
