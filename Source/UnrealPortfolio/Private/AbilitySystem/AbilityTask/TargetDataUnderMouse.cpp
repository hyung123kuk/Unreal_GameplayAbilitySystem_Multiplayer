// Copyright Druid Mechanics


#include "AbilitySystem/AbilityTask/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility, const TArray<AActor*> TargetActors)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	for (AActor* TargetActor : TargetActors)
	{
		MyObj->TargetActor.Add(TargetActor);
	}
	return MyObj;
}

void UTargetDataUnderMouse::SetCurrentPredictionKey(int16 PredictKeyCurrent)
{
	PredictionKeyCurrent = PredictKeyCurrent;
}

void UTargetDataUnderMouse::SetActivationTag(const FGameplayTag& GameplayTag)
{
	ActivationTag = GameplayTag;
}


void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		//�����Ͱ� ������ ���� �Լ� ȣ��
		FPredictionKey PredictionKey = GetActivationPredictionKey();
		PredictionKey.Current = PredictionKeyCurrent;
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);

		//�̹� �����Ͱ� ������ �Դٸ� �Լ� ȣ��
		const bool bCalledDelegate = AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey);
		if (!bCalledDelegate)
		{
			UE_LOG(LogTemp, Log, TEXT("(Server)Prev Arrived Client Data: %d"), PredictionKeyCurrent);

			SetWaitingOnRemotePlayerData();
		}
	}
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());

	APlayerController* PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult CursorHit;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);

	FGameplayAbilityTargetDataHandle DataHandle;
	FGameplayAbilityTargetData_SingleTargetHit* Data = new FGameplayAbilityTargetData_SingleTargetHit();
	Data->HitResult = CursorHit;

	FGameplayAbilityTargetData_ActorArray* TargetDataActors = new FGameplayAbilityTargetData_ActorArray();
	TargetDataActors->SetActors(TargetActor);

	DataHandle.Add(Data);
	DataHandle.Add(TargetDataActors);
	
	FPredictionKey PredictionKey = GetActivationPredictionKey();
	PredictionKey.Current = PredictionKeyCurrent;

	UE_LOG(LogTemp, Log, TEXT("SendClinet : %d"), PredictionKeyCurrent);

	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		PredictionKey,
		DataHandle,
		ActivationTag,
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle, ActivationTag);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ClientActivationTag)
{
	FPredictionKey PredictionKey = GetActivationPredictionKey();
	PredictionKey.Current = PredictionKeyCurrent;

	UE_LOG(LogTemp, Log, TEXT("Recieve Server : %d"), PredictionKeyCurrent);
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle, ClientActivationTag);
	}

	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), PredictionKey);
}
