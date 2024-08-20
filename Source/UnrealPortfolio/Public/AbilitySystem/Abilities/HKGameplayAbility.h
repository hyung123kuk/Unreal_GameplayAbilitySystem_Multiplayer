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
	

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	UFUNCTION()
	virtual void ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData);

	bool IsLocalPlayer();
	bool ServerProcess();
	bool IsStandAlone();
	bool IsListenServerCharacter();

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
	virtual void OnOccurMontageEvent(FGameplayEventData Payload);

	void PlayRandomActMontage(FGameplayTag Type);
	virtual void PlayMontage(UAnimMontage* MontageToPlay, FGameplayTag MontageEvent,bool EndAbilityWhenCompleteMontage = true);
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

protected:
	FTaggedMontage TaggedMontage;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AHKCharacterBase> CharacterBase;

public:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupInputTag;
	
};
