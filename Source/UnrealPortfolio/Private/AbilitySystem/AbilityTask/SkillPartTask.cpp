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

void USkillPartTask::InitSkillPartTask(UGameplayAbility* OwningAbility, const FSkillPartTaskInfo& TaskInfo)
{
	InitTask(*OwningAbility, OwningAbility->GetGameplayTaskDefaultPriority());
	Ability = OwningAbility;
	RunImmediatelyNextTask = TaskInfo.RunImmediatelyNextTask;
	TriggerTag = TaskInfo.TriggerTag;
	SocketTag = TaskInfo.SocketTag;
	SocketName = TaskInfo.SocketName;
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

	if (bFacingMousePositionWhenMotionWarping)
		FacingPosition(HitResult.Location);


	AActor* HitActor = HitResult.GetActor();
	if (HitActor != nullptr && HitActor->Implements<UCombatInterface>())
	{
		K2_CastMouseTargetSkillTask(HitActor, ActivationTag);
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

TArray<AActor*> USkillPartTask::FindTargetsWithAngle(const FVector& Origin, float Radius, const FVector& Direction, double Angle) const
{
	TArray<AActor*> RadiusActors = FindTargetsWithRadius(Origin, Radius);
	TArray<AActor*> TargetActors;
	for (AActor* RadiusActor : RadiusActors)
	{
		FVector TargetVector = RadiusActor->GetActorLocation() - Origin;
		float angle = cos(FMath::DegreesToRadians(Angle));
		float DotProduct = FVector::DotProduct(TargetVector.GetSafeNormal(), Direction.GetSafeNormal());
		if (angle < DotProduct)
		{
			TargetActors.Add(RadiusActor);
		}
	}

	return TargetActors;
}

TArray<AActor*> USkillPartTask::FindTargetsWithRadius(const FVector& Origin, float Radius) const
{
	TArray<FOverlapResult> Overlaps;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AABTA_SphereMultiTrace), false, GetAvatarActorFromActorInfo());
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Projectile, FCollisionShape::MakeSphere(Radius), Params);

	TArray<AActor*> HitActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		if (HitActor && HitActor->Implements<UCombatInterface>() && !IsSameTeam(HitActor, GetAvatarActorFromActorInfo()))
		{
			HitActors.Add(HitActor);
		}
	}

	return HitActors;
}

AActor* USkillPartTask::GetAvatarActorFromActorInfo() const
{
	return Ability->GetAvatarActorFromActorInfo();
}

bool USkillPartTask::IsLocalPlayer() const
{
	return SkillAbility->IsLocalPlayer();
}

void USkillPartTask::CauseDamage(AActor* TargetActor, float Damage) const
{
	SkillAbility->CauseDamage(TargetActor, Damage);
}

