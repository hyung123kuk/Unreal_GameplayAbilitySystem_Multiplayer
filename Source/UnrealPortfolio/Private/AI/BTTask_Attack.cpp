// Copyright Druid Mechanics


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interaction/CombatInterface.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BlueprintGameplayTagLibrary.h"
#include "HKGameplayTags.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Ret = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Ret != EBTNodeResult::Type::Succeeded)
		return Ret;

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	UObject* Obj = BlackboardComp->GetValue<UBlackboardKeyType_Object>(CombatTargetSelector.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(Obj);
	if (!IsValid(TargetActor))
	{
		return EBTNodeResult::Type::Failed;
	}
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OwningPawn);
	FGameplayTagContainer TagContainer = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(FHKGameplayTags::Get().Abilities_Attack);
	ASC->TryActivateAbilitiesByTag(TagContainer);

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(OwningPawn);
	if (CombatInterface)
	{
		CombatInterface->SetCombatTarget(TargetActor);
	}

	return EBTNodeResult::Type::Succeeded;
}

