// Copyright Druid Mechanics


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"


#include "AbilitySystemComponent.h"
#include "AbilitySystem/HKAttributeSet.h"
#include "HKGameplayTags.h"
#include "Character/CharacterClassInfo.h"
#include "AbilitySystem/HKAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "HKAbilityTypes.h"

struct HKDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(MissChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);


	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;

	HKDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHKAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHKAttributeSet, MissChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHKAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UHKAttributeSet, CriticalHitDamage, Source, false);

		const FHKGameplayTags& Tags = FHKGameplayTags::Get();

		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_Armor, ArmorDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_MissChance, MissChanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitChance, CriticalHitChanceDef);
		TagsToCaptureDefs.Add(Tags.Attribute_Secondary_CriticalHitDamage, CriticalHitDamageDef);

	}
};

static const HKDamageStatics& DamageStatics()
{
	static HKDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().MissChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();

	float TargetMissChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().MissChanceDef, EvaluationParameters, TargetMissChance);
	TargetMissChance = FMath::Max<float>(TargetMissChance, 0.f);

	const bool bMiss = FMath::RandRange(1, 100) < TargetMissChance;
	if (bMiss)
	{

		UHKAbilitySystemLibrary::SetIsMiss(EffectContextHandle, bMiss);
		return;
	}


	float Damage = Spec.GetSetByCallerMagnitude(FHKGameplayTags::Get().Damage);

	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	Damage -= TargetArmor;

	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0.f);
	const bool bCriticalHit = FMath::RandRange(1, 100) < SourceCriticalHitChance;

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0.f);

	UHKAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	Damage = bCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;

	FGameplayModifierEvaluatedData EvaluatedData(UHKAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
