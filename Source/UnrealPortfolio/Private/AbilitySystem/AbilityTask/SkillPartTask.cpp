// Copyright Druid Mechanics


#include "AbilitySystem/AbilityTask/SkillPartTask.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HKAbilitySystemLibrary.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "HKGameplayTags.h"
#include "UnrealPortfolio/UnrealPortfolio.h"
#include "Actor/HKProjectile.h"

void USkillPartTask::InitSkillPartTask(UGameplayAbility* OwningAbility, const FSkillPartTaskInfo& TaskInfo)
{
	InitTask(*OwningAbility, OwningAbility->GetGameplayTaskDefaultPriority());
	Ability = OwningAbility;
	RunImmediatelyNextTask = TaskInfo.RunImmediatelyNextTask;
	TriggerTag = TaskInfo.TriggerTag;
	SocketTag = TaskInfo.SocketTag;
	SocketName = TaskInfo.SocketName;
	MouseTargetAtSkiilStartUp = TaskInfo.MouseTargetAtSkiilStartUp;
	PredictionKeyCurrent = TaskInfo.PredictionKeyCurrent;
	ActorCombatInterface = Cast<ICombatInterface>(OwningAbility->GetAvatarActorFromActorInfo());
	Team = ActorCombatInterface->GetTeam();
	SkillAbility = Cast<UHKSkillAbilitiy>(OwningAbility);
}

void USkillPartTask::Activate()
{
	Super::Activate();
	UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(Ability, TriggerTag);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &USkillPartTask::OnOccurTriggerTag);
	UE_LOG(LogTemp, Log, TEXT("USkillPartTask[Activate] Wait For EventTag : %s"), *TriggerTag.GetTagName().ToString());
	WaitGameplayEventTask->ReadyForActivation();
}

void USkillPartTask::WaitGameplayEventTag(FGameplayTag Tag)
{
	UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(Ability, Tag);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &USkillPartTask::OnOccurTriggerTag);
	WaitGameplayEventTask->ReadyForActivation();
}

void USkillPartTask::OnOccurTriggerTag(const FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Log, TEXT("USkillPartTask[OnOccurTriggerTag] OnOccurTag : %s"), *TriggerTag.GetTagName().ToString());
	K2_OnOccurTriggerTag(Payload);

	if (!IsLocalPlayer())
	{
		//TODO : ∏ÛΩ∫≈Õ AI
	}
	else
	{
		TArray<AActor*> ActorArray = K2_TargetInfo(Payload.EventTag);
		UTargetDataUnderMouse* TargetData = UTargetDataUnderMouse::CreateTargetDataUnderMouse(Ability, ActorArray);
		TargetData->SetCurrentPredictionKey(PredictionKeyCurrent);
		TargetData->SetActivationTag(Payload.EventTag);
		TargetData->ValidData.AddDynamic(this, &USkillPartTask::Activate_TargetDataUnderMouse);
		UE_LOG(LogTemp, Log, TEXT("USkillPartTask[OnOccurTriggerTag] Wait For TargetData"));
		TargetData->ReadyForActivation();
	}
}

void USkillPartTask::Activate_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData, const FGameplayTag& ActivationTag)
{
	const FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, 0);

	K2_CastNonTargetSkillTask(HitResult, ActivationTag);

	if (bUseMotionWarpingToHitLocation)
		FacingPosition(HitResult.Location);

	AActor* HitActor = HitResult.GetActor();
	if (bTargetingWhenStartingMouseTarget)
	{
		if (MouseTargetAtSkiilStartUp != nullptr)
			K2_CastMouseTargetSkillTask(MouseTargetAtSkiilStartUp, ActivationTag);
	}
	else
	{
		if(HitActor != nullptr && HitActor->Implements<UCombatInterface>())
			K2_CastMouseTargetSkillTask(HitActor, ActivationTag);
		else if(MouseTargetAtSkiilStartUp != nullptr)
			K2_CastMouseTargetSkillTask(MouseTargetAtSkiilStartUp, ActivationTag);
	}

	const TArray<AActor*> TargetActors = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(TargetData, 1);
	if (TargetActors.Num() > 0)
	{
		K2_CastTargetArraySkillTask(TargetActors, ActivationTag);
	}

	OnEndSkillPartTask();
}


void USkillPartTask::OnEndSkillPartTask() const
{
	EndSkillPartTask.Broadcast();
}

bool USkillPartTask::IsSameTeam(AActor* Actor, AActor* Actor2) const
{
	return SkillAbility->IsSameTeam(Actor,Actor2);
}

void USkillPartTask::FacingPosition(const FVector& TargetPosition) const
{
	SkillAbility->FacingPosition(TargetPosition);
}

void USkillPartTask::FacingTarget() const
{
	SkillAbility->FacingTarget();
}

FVector USkillPartTask::GetSocketLocation(const FGameplayTag& Tag, const FName& Name) const
{
	return SkillAbility->GetSocketLocation(Tag, Name);
}

TArray<AActor*> USkillPartTask::FindTargetsWithAngle(const FVector& Origin, float Radius, const FVector& Direction, double Angle) const
{
	return SkillAbility->FindTargetsWithAngle(Origin, Radius, Direction, Angle);
}

TArray<AActor*> USkillPartTask::FindTargetsWithRadius(const FVector& Origin, float Radius) const
{
	return SkillAbility->FindTargetsWithRadius(Origin, Radius);
}

AActor* USkillPartTask::GetAvatarActorFromActorInfo() const
{
	return Ability->GetAvatarActorFromActorInfo();
}

bool USkillPartTask::IsLocalPlayer() const
{
	return SkillAbility->IsLocalPlayer();
}

bool USkillPartTask::ServerProcess() const
{
	return SkillAbility->ServerProcess();
}

bool USkillPartTask::IsStandAlone() const
{
	return SkillAbility->IsStandAlone();
}

bool USkillPartTask::IsListenServerCharacter() const
{
	return SkillAbility->IsListenServerCharacter();
}

void USkillPartTask::CauseDamage(AActor* TargetActor, float Damage) const
{
	SkillAbility->CauseDamage(TargetActor, Damage);
}

AHKProjectile* USkillPartTask::MakeProjectile(TSubclassOf<AHKProjectile> ProjectileClass, const FVector& ProjectileLocation, const FRotator& ProjectileRotation) const
{
	return SkillAbility->MakeProjectile(ProjectileClass, ProjectileLocation, ProjectileRotation);
}

FGameplayEffectSpecHandle USkillPartTask::MakeProjctileEffectSpecHandle(AHKProjectile* Projectile, const FVector& ProjectileTargetLocation, int Damage) const
{
	return SkillAbility->MakeProjctileEffectSpecHandle(Projectile, ProjectileTargetLocation, Damage);
}

void USkillPartTask::FinishSpawning(AHKProjectile* Projectile) const
{
	Projectile->FinishSpawning(Projectile->GetActorTransform());
}