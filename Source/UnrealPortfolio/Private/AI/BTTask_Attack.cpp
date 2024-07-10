// Copyright Druid Mechanics


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interaction/CombatInterface.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BlueprintGameplayTagLibrary.h"
#include "HKGameplayTags.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Ret = Super::ExecuteTask(OwnerComp, NodeMemory);
	if (Ret != EBTNodeResult::Type::Succeeded)
		return Ret;

	AActor* TargetActor = UBTFunctionLibrary::GetBlackboardValueAsActor(this, CombatTargetSelector);
	if (!IsValid(TargetActor))
	{
		return EBTNodeResult::Type::Failed;
	}

	ICombatInterface* CombatInterface = Cast<ICombatInterface>(ActorOwner);
	if (CombatInterface)
	{
		CombatInterface->SetCombatTarget(TargetActor);
	}

	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ActorOwner);
	FGameplayTagContainer TagContainer = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(FHKGameplayTags::Get().Abilities_Attack);
	ASC->TryActivateAbilitiesByTag(TagContainer);

	return EBTNodeResult::Type::Succeeded;
}

