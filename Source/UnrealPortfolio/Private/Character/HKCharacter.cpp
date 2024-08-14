// Copyright Druid Mechanics


#include "Character/HKCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "Player/HKPlayerState.h"
#include "AbilitySystemComponent.h"
#include "UI/HUD/HKHUD.h"
#include "Player/HKPlayerController.h"
#include "HKGameplayTags.h"
#include "GameframeWork/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Game/HKGameInstance.h"


AHKCharacter::AHKCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 800.f;
	ExpectedSpringArmLength = 800.f;
	SpringArmComponent->bUsePawnControlRotation = true;
	SpringArmComponent->bDoCollisionTest = true;
	SpringArmComponent->bInheritPitch = true;
	SpringArmComponent->bInheritYaw = true;
	SpringArmComponent->bInheritRoll = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	CameraComponent->bUsePawnControlRotation = false;
}

void AHKCharacter::BeginPlay()
{
	Super::BeginPlay();
	Team = FHKGameplayTags::Get().Team_1;
}

void AHKCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitAbilityActorInfo();
	AddCharacterAbilities(StartupAbilities);
}

void AHKCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void AHKCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (FMath::Abs(SpringArmComponent->TargetArmLength - ExpectedSpringArmLength) > KINDA_SMALL_NUMBER) 
	{
		SpringArmComponent->TargetArmLength = FMath::Lerp(SpringArmComponent->TargetArmLength, ExpectedSpringArmLength, 0.05f);
	}
}

void AHKCharacter::CameraZoomInZoomOut(float InputActionValue)
{
	if (InputActionValue > 0)
	{
		ExpectedSpringArmLength = FMath::Clamp<float>(SpringArmComponent->TargetArmLength - CameraWheelSpeed, SpringMinLength, SpringMaxLength);
	}
	else if (InputActionValue < 0)
	{
		ExpectedSpringArmLength = FMath::Clamp<float>(SpringArmComponent->TargetArmLength + CameraWheelSpeed, SpringMinLength, SpringMaxLength);
	}
}

void AHKCharacter::InitAbilityActorInfo()
{
	AHKPlayerState* HKPlayerState = GetPlayerState<AHKPlayerState>();
	check(HKPlayerState);
	HKPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(HKPlayerState, this);
	Cast<UHKAbilitySystemComponent>(HKPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = HKPlayerState->GetAbilitySystemComponent();
	AttributeSet = HKPlayerState->GetAttributeSet();

	InitializeDefaultAttributes();
}
