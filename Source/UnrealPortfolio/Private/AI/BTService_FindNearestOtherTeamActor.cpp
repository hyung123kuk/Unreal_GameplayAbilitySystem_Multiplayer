// Copyright Druid Mechanics


#include "AI/BTService_FindNearestOtherTeamActor.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "AbilitySystem/HKAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"

UBTService_FindNearestOtherTeamActor::UBTService_FindNearestOtherTeamActor()
{
	bNotifyTick = true;
}

void UBTService_FindNearestOtherTeamActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	TArray<AActor*> Enemies;	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(OwningPawn);
	check(CombatInterface);

	UHKAbilitySystemLibrary::GetLiveOtherTeamActors(OwningPawn, CombatInterface->GetTeam(), Enemies);
	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;

	for (AActor* Actor : Enemies)
	{
		if (IsValid(Actor) && IsValid(OwningPawn))
		{
			const float Distance = OwningPawn->GetDistanceTo(Actor);
			if (Distance < ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestActor = Actor;
			}
		}
	}

	
	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	BlackboardComp->SetValue<UBlackboardKeyType_Object>(TargetToFollowSelector.SelectedKeyName, ClosestActor);
	BlackboardComp->SetValue<UBlackboardKeyType_Float>(DistanceToTargetSelector.SelectedKeyName, ClosestDistance);

}

