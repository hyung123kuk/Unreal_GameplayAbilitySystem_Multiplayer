// Copyright Druid Mechanics


#include "AI/BTTask_GoAroundTarget.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

EBTNodeResult::Type UBTTask_GoAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Ret = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Ret != EBTNodeResult::Type::Succeeded)
		return Ret;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValue<UBlackboardKeyType_Object>(Target.SelectedKeyName));
	if (!IsValid(TargetActor))
	{
		return EBTNodeResult::Type::Failed;
	}

	FVector RandomLocation;
	if (!UNavigationSystemV1::K2_GetRandomLocationInNavigableRadius(this, TargetActor->GetActorLocation(), RandomLocation, Radius))
	{
		return EBTNodeResult::Type::Failed;
	}

	BlackboardComp->SetValueAsVector(NewLocation.SelectedKeyName ,RandomLocation);

	return EBTNodeResult::Type::Succeeded;
}
