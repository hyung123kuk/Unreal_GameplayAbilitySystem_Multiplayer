// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/HKCombatAbility.h"
#include "HKSkillAbilitiy.generated.h"


class USkillPartTask;

UENUM(BlueprintType)
enum class ESkillCategoryType : uint8
{
	Melee,
	ProjectileSpell,
	Magic,
	None,
};

USTRUCT(BlueprintType)
struct FSkillPartTaskInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<USkillPartTask> Task;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool RunImmediatelyNextTask; // 다음 Task 바로 실행

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag TriggerTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SocketName;

	UPROPERTY()
	int16 PredictionKeyCurrent;

	UPROPERTY()
	AActor* MouseTargetAtSkiilStartUp;

};
/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKSkillAbilitiy : public UHKCombatAbility
{
	GENERATED_BODY()
	
public:
	virtual bool GetLocalPlayerCondition(UHKAbilitySystemComponent* AbilitySystemComponent) override;
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData,const FGameplayTag& ActivationTag);
	virtual void OccurMontageEvent(const AActor* TargetActor, const FVector& CombatSocketLocation) override;
	UFUNCTION()
	virtual void CastSkill();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ESkillCategoryType Type;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* Montage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag EndTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FSkillPartTaskInfo> SkillPartInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool EndAbilityWhenCompleteMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool EndAbilityWhenCompleteTask;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUseMotionWarpingToHitLocation;

	UPROPERTY()
	AActor* MouseTargetAtSkiilStartUp;

private:

	int PartCount;

};
