// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "HKGameplayAbility.generated.h"

class AHKCharacterBase;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	

public:
	UFUNCTION(BlueprintCallable)
	bool IsLocalPlayer() const;
	UFUNCTION(BlueprintCallable)
	bool ServerProcess() const;
	UFUNCTION(BlueprintCallable)
	bool IsStandAlone() const;
	UFUNCTION(BlueprintCallable)
	bool IsListenServerCharacter() const;


	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	virtual void ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData,const FGameplayTag& ActivationTag);


	virtual void FindTargetDataUnderMouse();

	UFUNCTION()
	virtual void OnCompleteMontage();
	UFUNCTION()
	virtual void OnCancelledMontage();
	UFUNCTION()
	virtual void OnInterruptedMontage();

	virtual void OnCompleteAbility();
	virtual void OnFailedAbility();

	UFUNCTION()
	virtual void OnOccurMontageEvent(const FGameplayEventData Payload);

	void PlayRandomActMontage(const FGameplayTag& Type);
	virtual void PlayMontage(UAnimMontage* MontageToPlay, const FGameplayTag& MontageEvent,bool EndAbilityWhenCompleteMontage = true);
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

protected:
	FTaggedMontage TaggedMontage;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AHKCharacterBase> CharacterBase;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
	
};
