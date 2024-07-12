// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GoAroundTarget.generated.h"

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UBTTask_GoAroundTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector NewLocation;

	UPROPERTY(EditAnywhere)
	float Radius;
};
