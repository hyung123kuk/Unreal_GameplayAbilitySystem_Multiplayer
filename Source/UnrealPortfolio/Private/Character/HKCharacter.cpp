// Copyright Druid Mechanics


#include "Character/HKCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "Player/HKPlayerState.h"
#include "AbilitySystemComponent.h"
#include "UI/HUD/HKHUD.h"
#include "Player/HKPlayerController.h"
#include "HKGameplayTags.h"

AHKCharacter::AHKCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AHKCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	Team = FHKGameplayTags::Get().Team_1;
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AHKCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitAbilityActorInfo();
}

void AHKCharacter::InitAbilityActorInfo()
{
	AHKPlayerState* HKPlayerState = GetPlayerState<AHKPlayerState>();
	check(HKPlayerState);
	HKPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(HKPlayerState, this);
	Cast<UHKAbilitySystemComponent>(HKPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = HKPlayerState->GetAbilitySystemComponent();
	AttributeSet = HKPlayerState->GetAttributeSet();

	//TODO : HUD
	if (AHKPlayerController* HKPlayerController = Cast<AHKPlayerController>(GetController()))
	{
		if (AHKHUD* PlayerHUD = Cast<AHKHUD>(HKPlayerController->GetHUD()))
		{
			PlayerHUD->InitOverlay(HKPlayerController, HKPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
	InitializeDefaultAttributes();
}
