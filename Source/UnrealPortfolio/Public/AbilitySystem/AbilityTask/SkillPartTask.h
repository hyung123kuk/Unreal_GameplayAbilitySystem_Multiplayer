// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilitySystem/Abilities/HKSkillAbilitiy.h"
#include "Interaction/CombatInterface.h"
#include "SkillPartTask.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndSkillPartDelegate);


/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class UNREALPORTFOLIO_API USkillPartTask : public UAbilityTask
{
	GENERATED_BODY()
	
public:
	void InitSkillPartTask(UGameplayAbility* OwningAbility, const FSkillPartTaskInfo& TaskInfo);
	virtual void Activate() override;

	UFUNCTION()
	virtual void OnOccurTriggerTag(const FGameplayEventData Payload);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnOccurTriggerTag(FGameplayEventData Payload) const;


	UFUNCTION()
	void Activate_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData, const FGameplayTag& ActivationTag);

	UFUNCTION(BlueprintImplementableEvent)
	TArray<AActor*> K2_TargetInfo(const FGameplayTag ActivationTag) const;

	UFUNCTION(BlueprintImplementableEvent)
	void K2_CastNonTargetSkillTask(const FHitResult HitResult,const FGameplayTag ActivationTag) const;

	UFUNCTION(BlueprintImplementableEvent)
	void K2_CastMouseTargetSkillTask(const AActor* HitResult, const FGameplayTag ActivationTag) const;

	UFUNCTION(BlueprintImplementableEvent)
	void K2_CastTargetArraySkillTask(const TArray<AActor*>& TargetArray, const FGameplayTag ActivationTag) const;

	UPROPERTY()
	FEndSkillPartDelegate EndSkillPartTask;
	
private:
	UFUNCTION(BlueprintCallable)
	void WaitGameplayEventTag(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable)
	FVector GetSocketLocation(const FGameplayTag& SocketTag, const FName& SocketName) const;

	UFUNCTION(BlueprintCallable)
	TArray<AActor*> FindTargetsWithAngle(const FVector& Origin, float Radius, const FVector& Direction, double Angle) const;
	
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> FindTargetsWithRadius(const FVector& Origin, float Radius) const;

	UFUNCTION(BlueprintCallable)
	AActor* GetAvatarActorFromActorInfo() const;


	UFUNCTION(BlueprintCallable)
	bool IsSameTeam(AActor* Actor, AActor* Actor2) const;
	
	UFUNCTION(BlueprintCallable)
	void OnEndSkillPartTask() const;

	UFUNCTION(BlueprintCallable)
	void FacingPosition(const FVector& TargetPosition) const;

	UFUNCTION(BlueprintCallable)
	void FacingTarget() const;

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor, float Damage) const;

	UFUNCTION(BlueprintCallable)
	AHKProjectile* MakeProjectile(TSubclassOf<AHKProjectile> ProjectileClass, const FVector& ProjectileLocation, const FRotator& ProjectileRotation) const;
	
	UFUNCTION(BlueprintCallable)
	FGameplayEffectSpecHandle MakeProjctileEffectSpecHandle(AHKProjectile* Projectile, const FVector& ProjectileTargetLocation, int Damage) const;

	UFUNCTION(BlueprintCallable)
	bool IsLocalPlayer() const;

	UFUNCTION(BlueprintCallable)
	bool ServerProcess() const;

	UFUNCTION(BlueprintCallable)
	bool IsStandAlone() const;

	UFUNCTION(BlueprintCallable)
	bool IsListenServerCharacter() const;

	UFUNCTION(BlueprintCallable)
	void FinishSpawning(AHKProjectile* Projectile) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bFacingMousePositionWhenMotionWarping; // 마우스 포지션으로 모션워핑

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bTargetingMouseToAbilityStartTarget; //어빌리티 시작시 타겟을 공격 타겟으로 설정

	UPROPERTY(BlueprintReadOnly)
	AActor* MouseTargetAtSkiilStartUp;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag Team;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag TriggerTag;

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(BlueprintReadOnly)
	FName SocketName;

private:
	UHKSkillAbilitiy* SkillAbility;

	ICombatInterface* ActorCombatInterface;

	bool RunImmediatelyNextTask; // 다음 Task 바로 실행

	int16 PredictionKeyCurrent;
};
