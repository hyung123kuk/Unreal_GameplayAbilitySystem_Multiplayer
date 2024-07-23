// Copyright Druid Mechanics


#include "Character/HKEnemy.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "AbilitySystem/HKAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/HKAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AbilitySystem/HKAbilitySystemLibrary.h"
#include "UI/HKUserWidget.h"
#include "Components/WidgetComponent.h"
#include "HKGameplayTags.h"
#include "UnrealPortfolio/UnrealPortfolio.h"

AHKEnemy::AHKEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	AbilitySystemComponent = CreateDefaultSubobject<UHKAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	AttributeSet = CreateDefaultSubobject<UHKAttributeSet>("AttributeSet");

	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	HealthBar->SetupAttachment(GetRootComponent());
}

void AHKEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);


	if (!HasAuthority()) return;
	HKAIController = Cast<AHKAIController>(NewController);
	HKAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	HKAIController->RunBehaviorTree(BehaviorTree);
	HKAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);
	HKAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), !bMeleeAttack);
}

void AHKEnemy::HighlightActor()
{
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
}

void AHKEnemy::UnHighlightActor()
{
	GetMesh()->SetRenderCustomDepth(false);
}

AActor* AHKEnemy::GetTarget()
{
	return this;
}


void AHKEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	if (HKAIController) HKAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"), true);

	Super::Die();
}

void AHKEnemy::BeginPlay()
{
	Super::BeginPlay();
	Team = FHKGameplayTags::Get().Team_2;
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UHKAbilitySystemLibrary::GiveStartupAbilities(this, AbilitySystemComponent, CharacterClass);
	}


	if (UHKUserWidget* HKUserWidget = Cast<UHKUserWidget>(HealthBar->GetUserWidgetObject()))
	{
		HKUserWidget->SetWidgetController(this);
	}

	if (const UHKAttributeSet* AS = Cast<UHKAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AS->GetMaxHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChanged.Broadcast(Data.NewValue);
			}
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(FHKGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(
			this,
			&AHKEnemy::HitReactTagChanged
		);

		OnHealthChanged.Broadcast(AS->GetHealth());
		OnMaxHealthChanged.Broadcast(AS->GetMaxHealth());
	}
}

void AHKEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (HKAIController && HKAIController->GetBlackboardComponent())
	{
		HKAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), bHitReacting);
	}
}

void AHKEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	Cast<UHKAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();

	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AHKEnemy::InitializeDefaultAttributes() const
{
	UHKAbilitySystemLibrary::InitializeDefaultAttributes(this, CharacterClass, 0, AbilitySystemComponent);
}
