// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindNearestOtherTeamActor.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UBTService_FindNearestOtherTeamActor : public UBTService
{
	GENERATED_BODY()
	
protected:

	UBTService_FindNearestOtherTeamActor();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector TargetToFollowSelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FBlackboardKeySelector DistanceToTargetSelector;
	
};
