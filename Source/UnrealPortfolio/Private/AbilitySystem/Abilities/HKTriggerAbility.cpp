// Copyright Druid Mechanics


#include "AbilitySystem/Abilities/HKTriggerAbility.h"
#include "AbilitySystem/AbilityTask/TargetDataItem.h"
#include "AbilitySystem/HKGameplayAbilityTargetData.h"

void UHKTriggerAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	int ID = 0;
	int UniqueID = 0;
	K2_SetDataInformation(IN ID,IN UniqueID);
	FindTriggerData(ID, UniqueID);
}

void UHKTriggerAbility::FindTriggerData(int ID, int UniqueID)
{
	if (IsLocalPlayer())
	{
		UTargetDataItem* TargetDataTask = UTargetDataItem::CreateTargetDataUnderMouse(this);
		TargetDataTask->ID = ID;
		TargetDataTask->UniqueID = UniqueID;
		TargetDataTask->ValidData.AddDynamic(this, &UHKTriggerAbility::ActivateAbility_TargetData);
		TargetDataTask->ReadyForActivation();
	}
}

void UHKTriggerAbility::ActivateAbility_TargetData(const FGameplayAbilityTargetDataHandle& TargetDataHandle)
{
	int ID = -1;
	int UniqueID = 0;
	if (TargetDataHandle.Data.IsValidIndex(0))
	{
		FGameplayAbilityTargetData* Data = TargetDataHandle.Data[0].Get();
		UScriptStruct* DataStruct = Data->GetScriptStruct();
		ID = *DataStruct->FindPropertyByName("ID")->ContainerPtrToValuePtr<int>(Data);
		UniqueID = *DataStruct->FindPropertyByName("UniqueID")->ContainerPtrToValuePtr<int>(Data);
	}

	K2_ActivateAbility_TargetData(ID,UniqueID);
	OnCompleteAbility();
}
