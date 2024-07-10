// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "HKGameplayTags.h"
#include "Animnotify_EventTag.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "EventGameplayTag"))
class UNREALPORTFOLIO_API UAnimnotify_EventTag : public UAnimNotify
{
	GENERATED_BODY()
	
protected:

	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag;

};
