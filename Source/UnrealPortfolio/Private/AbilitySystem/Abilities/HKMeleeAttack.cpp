// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKMeleeAttack.h"
#include "Interaction/CombatInterface.h"
#include "HKGameplayTags.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/HitResult.h"
#include "AbilitySystem/HKAbilitySystemLibrary.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"

void UHKMeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!IsLocalPlayer())
	{
		MeleeAttack();
	}
}

void UHKMeleeAttack::ActivateAbility_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData)
{
	Super::ActivateAbility_TargetDataUnderMouse(TargetData);
	FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData,0);
	if (!HitResult.bBlockingHit)
		return;

	AActor* HitActor = HitResult.GetActor();
	if (HitActor == nullptr || !HitActor->Implements<UCombatInterface>())
	{
		return;
	}

	if (IsSameTeam(GetAvatarActorFromActorInfo(), HitActor))
		return;

	ICombatInterface* AbilityActorCombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	AbilityActorCombatInterface->SetCombatTarget(HitActor);

	MeleeAttack();
}

void UHKMeleeAttack::MeleeAttack()
{
	FacingTarget();
	PlayRandomAttackMontage(FHKGameplayTags::Get().AttackType_Melee);
}

void UHKMeleeAttack::OccurMontageEvent(const AActor* AvatarActor, const FVector& CombatSocketLocation)
{
	TArray<AActor*> OutOverlappingActors;

	UHKAbilitySystemLibrary::GetLiveOtherTeamActorsWithinRadius(this, Team, OutOverlappingActors, AttackRange, CombatSocketLocation);

	FGameplayCueParameters CueParam;
	CueParam.Location = CombatSocketLocation;
	CueParam.EffectCauser = GetAvatarActorFromActorInfo();
	CueParam.SourceObject = ActorCombatInterface->GetCombatTarget();
	GetAbilitySystemComponentFromActorInfo()->ExecuteGameplayCue(FHKGameplayTags::Get().GameplayCue_MeleeImpact, CueParam);
	for (auto Enemy : OutOverlappingActors)
	{
		CauseDamage(Enemy, Damage);

	}
	
	if (OutOverlappingActors.Num() > 0)
	{
		DrawDebugSphere(GetWorld(), CombatSocketLocation, AttackRange, 16, FColor::Green,false , 1.5f);
	}
	else
	{
		DrawDebugSphere(GetWorld(), CombatSocketLocation, AttackRange, 16, FColor::Red, false, 1.5f);
	}

}



