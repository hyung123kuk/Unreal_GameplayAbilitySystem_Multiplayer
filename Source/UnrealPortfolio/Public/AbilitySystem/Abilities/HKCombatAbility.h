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
	

	bool IsSameTeam(AActor* Actor, AActor* Actor2);
	bool PlayRandomAttackMontage(FGameplayTag AttackType);

	void FacingPosition(const FVector& TargetPosition);
	void FacingTarget();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	virtual bool GetLocalPlayerCondition(UHKAbilitySystemComponent* AbilitySystemComponent) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void FindTargetDataUnderMouse() override;

	virtual void OccurMontageEvent(const AActor* TargetActor, const FVector& CombatSocketLocation);

protected:

	FGameplayTag Team;
	ICombatInterface* ActorCombatInterface;

	TObjectPtr<AActor> Target;

	UPROPERTY(EditDefaultsOnly, Category = "LockON")
	bool bLockOn; //Ÿ�� LockOn
	UPROPERTY(EditDefaultsOnly, Category = "LockON")
	float LockOnCloserRange = 30.f; //Ÿ�� LockOn ���� ��Ÿ� ����
	UPROPERTY(EditDefaultsOnly, Category = "CombatRange")
	bool bCloserMouseTarget; //���콺�� �ش� Ÿ�� ���� ������
	UPROPERTY(EditDefaultsOnly, Category = "CombatRange")
	float CombatRange; //bCloserMouseTarget = true�� ��Ÿ�


private:
	virtual void OnOccurMontageEvent(FGameplayEventData Payload) override;
};
