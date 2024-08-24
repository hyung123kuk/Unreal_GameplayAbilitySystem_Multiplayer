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
	void InitSkillPartTask(UGameplayAbility* OwningAbility, FSkillPartTaskInfo& TaskInfo);
	virtual void Activate() override;

	UFUNCTION()
	virtual void OnOccurTriggerTag(FGameplayEventData Payload);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_OnOccurTriggerTag(FGameplayEventData Payload);


	UFUNCTION()
	void Activate_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData);

	UFUNCTION(BlueprintImplementableEvent)
	TArray<AActor*> K2_TargetInfo();

	UFUNCTION(BlueprintImplementableEvent)
	void K2_CastNonTargetSkillTask(const FHitResult HitResult);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_CastMouseTargetSkillTask(const AActor* HitResult);

	UFUNCTION(BlueprintImplementableEvent)
	void K2_CastTargetArraySkillTask(const TArray<AActor*>& TargetArray);

	UPROPERTY()
	FEndSkillPartDelegate EndSkillPartTask;
	
private:
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> FindTargetsWithAngle(const FVector& Origin, float Radius, const FVector& Direction, double Angle);
	
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> FindTargetsWithRadius(const FVector& Origin, float Radius);

	UFUNCTION(BlueprintCallable)
	AActor* GetAvatarActorFromActorInfo();

	UFUNCTION(BlueprintCallable)
	bool IsLocalPlayer();

	UFUNCTION(BlueprintCallable)
	bool IsSameTeam(AActor* Actor, AActor* Actor2);
	
	UFUNCTION(BlueprintCallable)
	void OnEndSkillPartTask();

	void FacingPosition(const FVector& TargetPosition);
	void FacingTarget();

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor, float Damage);



private:
	UHKSkillAbilitiy* SkillAbility;

	FGameplayTag Team;

	ICombatInterface* ActorCombatInterface;

	UPROPERTY()
	bool RunImmediatelyNextTask; // 다음 Task 바로 실행

	UPROPERTY()
	FGameplayTag TriggerTag;

	UPROPERTY()
	FGameplayTag SocketTag;

	UPROPERTY()
	FName SocketName;	
	
};
