// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HKCombatAbility.h"
#include "HKProjectileSpell.generated.h"


class UGameplayEffect;
class AHKProjectile;
struct FGameplayTag;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKProjectileSpell : public UHKCombatAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData);
	
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo);

	void PlaySpawnProjectileMontage();

	virtual void OccurMontageEvent(const AActor* AvatarActor, const FVector& CombatSocketLocation) override;


	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AHKProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Released Spawn")
	bool bMouseReleasedSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "Option")
	bool bOverridePitch;

	UPROPERTY(EditDefaultsOnly, Category = "Option")
	float PitchOverride;

};
