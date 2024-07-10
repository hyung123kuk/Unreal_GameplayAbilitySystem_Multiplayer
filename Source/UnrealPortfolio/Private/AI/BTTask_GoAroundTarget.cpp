// Copyright Druid Mechanics


#include "AI/BTTask_GoAroundTarget.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "NavigationSystem.h"

EBTNodeResult::Type UBTTask_GoAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Ret = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Ret != EBTNodeResult::Type::Succeeded)
		return Ret;

	AActor* TargetActor = UBTFunctionLibrary::GetBlackboardValueAsActor(this, Target);
	if (!IsValid(TargetActor))
	{
		return EBTNodeResult::Type::Failed;
	}

	FVector RandomLocation;
	if (!UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, TargetActor->GetActorLocation(), RandomLocation, Radius))
	{
		return EBTNodeResult::Type::Failed;
	}

	UBTFunctionLibrary::SetBlackboardValueAsVector(this, NewLocation, RandomLocation);

	return EBTNodeResult::Type::Succeeded;
}
