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
	TArray<AActor*> FindTargetsWithAngle(const FVector& Origin, float Radius, const FVector& Direction, double Angle) const;
	
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> FindTargetsWithRadius(const FVector& Origin, float Radius) const;

	UFUNCTION(BlueprintCallable)
	AActor* GetAvatarActorFromActorInfo() const;

	UFUNCTION(BlueprintCallable)
	bool IsLocalPlayer() const;

	UFUNCTION(BlueprintCallable)
	bool IsSameTeam(AActor* Actor, AActor* Actor2) const;
	
	UFUNCTION(BlueprintCallable)
	void OnEndSkillPartTask() const;

	void FacingPosition(const FVector& TargetPosition) const;
	void FacingTarget() const;

	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor, float Damage) const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bFacingMousePositionWhenMotionWarping;

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

	UPROPERTY()
	int16 PredictionKeyCurrent;
};
