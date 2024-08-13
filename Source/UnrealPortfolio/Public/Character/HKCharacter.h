// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Character/HKCharacterBase.h"
#include "HKCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKCharacter : public AHKCharacterBase
{
	GENERATED_BODY()
public:
	AHKCharacter();
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void Tick(float DeltaSeconds) override;

	void CameraZoomInZoomOut(float InputActionValue);


	
private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm", Meta = (AllowPrivateAccess = "true"))
	float SpringMinLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm", Meta = (AllowPrivateAccess = "true"))
	float SpringMaxLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringArm", Meta = (AllowPrivateAccess = "true"))
	float CameraWheelSpeed;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "SpringArm", Meta = (AllowPrivateAccess = "true"))
	float ExpectedSpringArmLength; //스프링암 길이 목표값.

	virtual void InitAbilityActorInfo() override;

};
