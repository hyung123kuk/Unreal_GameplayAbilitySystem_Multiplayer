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

		//�����Ͱ� ������ ���� �Լ� ȣ��
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).AddUObject(this, &UTargetDataItem::OnTargetDataReplicatedCallback);

		//�̹� �����Ͱ� ������ �Դٸ� �Լ� ȣ��
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