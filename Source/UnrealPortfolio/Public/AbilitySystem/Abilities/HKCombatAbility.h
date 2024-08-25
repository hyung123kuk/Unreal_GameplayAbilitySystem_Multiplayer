// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HKGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/AbilityInterface.h"
#include "HKCombatAbility.generated.h"


class AHKProjectile;
/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKCombatAbility : public UHKGameplayAbility, public IAbilityInterface
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor, float Damage) const;
	
	UFUNCTION(BlueprintCallable)
	bool IsSameTeam(AActor* Actor, AActor* Actor2) const;
	UFUNCTION(BlueprintCallable)
	bool PlayRandomAttackMontage(const FGameplayTag& AttackType);
	UFUNCTION(BlueprintCallable)
	AHKProjectile* MakeProjectile(TSubclassOf<AHKProjectile> ProjectileClass, const FVector& ProjectileLocation, const FRotator& ProjectileRotation) const;
	UFUNCTION(BlueprintCallable)
	FGameplayEffectSpecHandle MakeProjctileEffectSpecHandle(AHKProjectile* Projectile, const FVector& ProjectileTargetLocation, int Damage) const;
	UFUNCTION(BlueprintCallable)
	void FacingPosition(const FVector& TargetPosition) const;
	UFUNCTION(BlueprintCallable)
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
	bool bLockOn; //타겟 LockOn
	UPROPERTY(EditDefaultsOnly, Category = "LockON")
	float LockOnCloserRange = 30.f; //타겟 LockOn 시전 사거리 감소
	UPROPERTY(EditDefaultsOnly, Category = "CombatRange")
	bool bCloserMouseTarget; //마우스의 해당 타겟 까지 가야함
	UPROPERTY(EditDefaultsOnly, Category = "CombatRange")
	float CombatRange; //bCloserMouseTarget = true시 사거리


private:
	virtual void OnOccurMontageEvent(const FGameplayEventData Payload) override;
};
