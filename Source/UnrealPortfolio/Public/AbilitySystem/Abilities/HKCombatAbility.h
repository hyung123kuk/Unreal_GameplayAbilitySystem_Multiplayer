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

	void FacingTarget();
	virtual void OccurMontageEvent(const AActor* TargetActor, const FVector& CombatSocketLocation);

	bool ServerProcess();
	bool IsStandAlone();
	bool IsListenServerCharacter();


protected:

	FGameplayTag Team;
	ICombatInterface* ActorCombatInterface;

	TObjectPtr<AActor> Target;

	UPROPERTY(EditDefaultsOnly, Category = "CombatRange")
	bool bCloserMouseTarget;
	UPROPERTY(EditDefaultsOnly, Category = "CombatRange")
	float CombatRange;

private:
	virtual void OnOccurMontageEvent(FGameplayEventData Payload) override;
};
