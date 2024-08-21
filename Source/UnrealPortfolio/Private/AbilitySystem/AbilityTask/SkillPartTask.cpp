// Copyright Druid Mechanics


#include "AbilitySystem/AbilityTask/SkillPartTask.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/HKAbilitySystemLibrary.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "HKGameplayTags.h"

void USkillPartTask::InitSkillPartTask(UGameplayAbility* OwningAbility, FSkillPartTaskInfo& TaskInfo)
{
	InitTask(*OwningAbility, OwningAbility->GetGameplayTaskDefaultPriority());
	Ability = OwningAbility;
	RunImmediatelyNextTask = TaskInfo.RunImmediatelyNextTask;
	TriggerTag = TaskInfo.TriggerTag;
	SocketTag = TaskInfo.SocketTag;
	SocketName = TaskInfo.SocketName;
	ActorCombatInterface = Cast<ICombatInterface>(OwningAbility->GetAvatarActorFromActorInfo());
	Team = ActorCombatInterface->GetTeam();
}

void USkillPartTask::Activate()
{
	Super::Activate();
	UAbilityTask_WaitGameplayEvent* WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(Ability, TriggerTag);
	WaitGameplayEventTask->EventReceived.AddDynamic(this, &USkillPartTask::OnOccurTriggerTag);
	UE_LOG(LogTemp, Log, TEXT("USkillPartTask[Activate] Wait For EventTag : %s"), *TriggerTag.GetTagName().ToString());
	WaitGameplayEventTask->ReadyForActivation();
}

void USkillPartTask::OnOccurTriggerTag(FGameplayEventData Payload)
{
	UE_LOG(LogTemp, Log, TEXT("USkillPartTask[OnOccurTriggerTag] OnOccurTag : %s"), *TriggerTag.GetTagName().ToString());
	K2_OnOccurTriggerTag(Payload);

	if (!IsLocalPlayer())
	{
		//TODO : ∏ÛΩ∫≈Õ AI
	}
	else
	{
		TArray<AActor*> ActorArray = K2_TargetInfo();
		UTargetDataUnderMouse* TargetData = UTargetDataUnderMouse::CreateTargetDataUnderMouse(Ability, ActorArray);
		TargetData->ValidData.AddDynamic(this, &USkillPartTask::Activate_TargetDataUnderMouse);
		UE_LOG(LogTemp, Log, TEXT("USkillPartTask[OnOccurTriggerTag] Wait For TargetData"));
		TargetData->ReadyForActivation();
	}
	
}

void USkillPartTask::Activate_TargetDataUnderMouse(const FGameplayAbilityTargetDataHandle& TargetData)
{
	UE_LOG(LogTemp, Log, TEXT("USkillPartTask[Activate_TargetDataUnderMouse] Arrived TargetDataInfo"));
	const FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(TargetData, 0);

	K2_CastNonTargetSkillTask(HitResult);
	FacingPosition(HitResult.Location);


	AActor* HitActor = HitResult.GetActor();
	if (HitActor != nullptr)
	{
		K2_CastMouseTargetSkillTask(HitActor);
		ActorCombatInterface->SetCombatTarget(HitActor);
	}

	const TArray<AActor*> TargetActors = UAbilitySystemBlueprintLibrary::GetActorsFromTargetData(TargetData, 1);
	if (TargetActors.Num() > 0)
	{
		K2_CastTargetArraySkillTask(TargetActors);
	}

	OnEndSkillPartTask();
}


void USkillPartTask::OnEndSkillPartTask()
{
	EndSkillPartTask.Broadcast();
}

bool USkillPartTask::IsSameTeam(AActor* Actor, AActor* Actor2)
{
	return UHKAbilitySystemLibrary::IsSameTeam(Actor, Actor2);
}

void USkillPartTask::FacingPosition(const FVector& TargetPosition)
{
	ActorCombatInterface->Execute_UpdateFacingTarget(Ability->GetAvatarActorFromActorInfo(), TargetPosition);
}

void USkillPartTask::FacingTarget()
{
	AActor* Target = ActorCombatInterface->GetCombatTarget();
	if (Target != nullptr)
	{
		ActorCombatInterface->Execute_UpdateFacingTarget(Ability->GetAvatarActorFromActorInfo(), Target->GetActorLocation());
	}
}

TArray<AActor*> USkillPartTask::FindTargetsWithAngle(const FVector& Origin, float Radius, const FVector& Direction, double Angle)
{
	TArray<AActor*> RadiusActor = FindTargetsWithRadius(Origin, Radius);
	TArray<AActor*> TargetActors;
	for (AActor* Target : RadiusActor)
	{
		FVector TargetVector = Target->GetActorLocation() - Origin;
		if (cos(Angle) < FVector::DotProduct(TargetVector.GetSafeNormal(), Direction.GetSafeNormal()))
		{
			TargetActors.Add(Target);
		}
	}

	return TargetActors;
}

TArray<AActor*> USkillPartTask::FindTargetsWithRadius(const FVector& Origin, float Radius)
{
	TArray<FOverlapResult> Overlaps;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(AABTA_SphereMultiTrace), false, Ability->GetAvatarActorFromActorInfo());
	GetWorld()->OverlapMultiByChannel(Overlaps, Origin, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(Radius), Params);

	TArray<AActor*> HitActors;
	for (const FOverlapResult& Overlap : Overlaps)
	{
		AActor* HitActor = Overlap.OverlapObjectHandle.FetchActor<AActor>();
		if (HitActor && !HitActors.Contains(HitActor))
		{
			HitActors.Add(HitActor);
		}
	}

	return HitActors;
}

AActor* USkillPartTask::GetAvatarActorFromActorInfo()
{
	return Ability->GetAvatarActorFromActorInfo();
}

bool USkillPartTask::IsLocalPlayer()
{
	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (PC == nullptr)
	{
		return false;
	}
	return true;
}

void USkillPartTask::CauseDamage(AActor* TargetActor, float Damage)
{
	Cast<UHKCombatAbility>(Ability)->CauseDamage(TargetActor,Damage);
}

