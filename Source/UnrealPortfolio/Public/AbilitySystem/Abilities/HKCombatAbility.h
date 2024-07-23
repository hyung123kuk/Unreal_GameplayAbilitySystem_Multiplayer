// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HKGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/AbilityInterface.h"
#include "HKCombatAbility.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKCombatAbility : public UHKGameplayAbility, public IAbilityInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor, float Damage);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	virtual bool GetLocalPlayerCondition(UHKAbilitySystemComponent* AbilitySystemComponent) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void FindTargetDataUnderMouse() override;

	bool IsSameTeam(const AActor* Actor,const AActor* Actor2);
	bool PlayRandomAttackMontage(FGameplayTag AttackType);
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	void FacingTarget();

	virtual void PlayMontage(UAnimMontage* MontageToPlay, FGameplayTag MontageEvent);

	UFUNCTION()
	virtual void OnCompleteMontage();
	UFUNCTION()
	virtual void OnCancelledMontage();
	UFUNCTION()
	virtual void OnInterruptedMontage();

	virtual void OccurMontageEvent(const AActor* TargetActor, const FVector& CombatSocketLocation);

protected:
	FTaggedMontage TaggedMontage;
	FGameplayTag Team;
	ICombatInterface* ActorCombatInterface;

	TObjectPtr<AActor> Target;

	UPROPERTY(EditDefaultsOnly, Category = "CombatRange")
	bool bEndAbilityOutOfCombatRange;
	UPROPERTY(EditDefaultsOnly, Category = "CombatRange")
	float CombatRange;

private:
	UFUNCTION()
	void OnOccurMontageEvent(FGameplayEventData Payload);
};
