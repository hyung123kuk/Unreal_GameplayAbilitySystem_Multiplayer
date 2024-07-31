// Copyright Druid Mechanics


#include "AbilitySystem/AbilityTask/TargetDataItem.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/HKGameplayAbilityTargetData.h"

UTargetDataItem* UTargetDataItem::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataItem* MyObj = NewAbilityTask<UTargetDataItem>(OwningAbility);
	return MyObj;
}

void UTargetDataItem::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendItemData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		const FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();

		//데이터가 서버에 오면 함수 호출
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataItem::OnTargetDataReplicatedCallback);

		//이미 데이터가 서버에 왔다면 함수 호출
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataItem::SendItemData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	FGameplayAbilityTargetDataHandle DataHandle;

	FHKGameplayAbilityTargetData_Item* ItemData = new FHKGameplayAbilityTargetData_Item();
	ItemData->ID = ID;
	ItemData->UniqueID = UniqueID;

	DataHandle.Add(ItemData);

	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag(),
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataItem::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}