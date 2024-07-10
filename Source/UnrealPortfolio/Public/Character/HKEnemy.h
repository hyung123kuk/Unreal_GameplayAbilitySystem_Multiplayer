// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/HKCharacterBase.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/MouseTargetActorInterface.h"
#include "UI/WidgetController/InGame/OverlayWidgetController.h"
#include "HKEnemy.generated.h"

class AHKAIController;
class UWidgetComponent;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKEnemy : public AHKCharacterBase, public IMouseTargetActorInterface
{
	GENERATED_BODY()
	
public:
	AHKEnemy();
	virtual void PossessedBy(AController* NewController) override;
	
	/** Enemy Interface */
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;

	/**Enemy Interface End*/

	/** Combat Interface */
	virtual void Die() override;
	/**Combat Interface End*/

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;



protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY()
	TObjectPtr<AHKAIController> HKAIController;
};
