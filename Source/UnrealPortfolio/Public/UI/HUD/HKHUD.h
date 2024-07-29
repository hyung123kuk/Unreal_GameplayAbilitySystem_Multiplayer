// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AbilitySystemComponent.h"
#include "HKHUD.generated.h"

class UHKUserWidget;
class UOverlayWidgetController;
struct FGASWidgetControllerParams;

/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API AHKHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	UOverlayWidgetController* GetOverlayWidgetController(const FGASWidgetControllerParams& WCParams);
	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

private:
	UPROPERTY()
	TObjectPtr<UHKUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UHKUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;


};
