// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HKGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "HKAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKAttackAbility : public UHKGameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor, float Damage);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	bool PlayRandomAttackMontage(FGameplayTag AttackType);
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	void FacingTarget();
	virtual void PlayMontage(UAnimMontage* MontageToPlay, FGameplayTag MontageEvent, float Rate = 1.f, FName StartSection = NAME_None);

	virtual void OnCompleteMontage();
	virtual void OnCancelledMontage();
	virtual void OnInterruptedMontage();
	virtual void OccurMontageEvent(const AActor* AvatarActor, const FVector& CombatSocketLocation);

protected:
	FTaggedMontage TaggedMontage;

private:
	void OccurMontageEvent(FGameplayEventData Payload);
};
