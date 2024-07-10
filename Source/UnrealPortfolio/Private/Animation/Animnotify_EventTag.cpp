// Copyright Druid Mechanics


#include "Animation/Animnotify_EventTag.h"
#include "AbilitySystemBlueprintLibrary.h"

FString UAnimnotify_EventTag::GetNotifyName_Implementation() const
{
	FString NotifyName = EventTag.ToString().Replace(TEXT("Montage."), TEXT(""));
	return NotifyName;
}

void UAnimnotify_EventTag::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Owner, EventTag, FGameplayEventData());

}
