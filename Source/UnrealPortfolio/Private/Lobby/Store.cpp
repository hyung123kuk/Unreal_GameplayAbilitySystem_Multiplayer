// Copyright Druid Mechanics


#include "Lobby/Store.h"
#include "HKDatabaseFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "HKAssetManager.h"
#include "UI/WidgetController/StoreWidgetController.h"
#include "Lobby/HKUILobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"



AStore::AStore()
{
	PrimaryActorTick.bCanEverTick = false;
	bAlwaysRelevant = true;
	bReplicates = true;
	NetUpdateFrequency = 10.f;
}

void AStore::BeginPlay()
{
	Super::BeginPlay();

	if (GetNetMode() == NM_DedicatedServer)
		return;

	SendChangedStoreInformationToClients();
}

void AStore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ItemsInfo);
}



bool AStore::GetItemGoldWithItemID(int ItemID, int& Gold)
{
	for (FStoreItemDefaultInfo ItemInfo : ItemsInfo)
	{
		if (ItemInfo.Id == ItemID)
		{
			Gold = ItemInfo.Cost;
			return true;
		}
	}

	return false;
}


void AStore::SendChangedStoreInformationToClients()
{
	AHKUILobbyPlayerController* LocalClientPlayerController = Cast<AHKUILobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!IsValid(LocalClientPlayerController))
	{
		return;
	}

	if (StoreInfoWidgetController == nullptr)
	{
		StoreInfoWidgetController = NewObject<UStoreWidgetController>(this, UStoreWidgetController::StaticClass());
	}
	LocalClientPlayerController->SetStoreWidgetController(StoreInfoWidgetController);
	StoreInfoWidgetController->SetWidgetControllerParams(ItemsInfo);
}

void AStore::OnRep_ItemInfo()
{
	for (int i = 0; i < ItemsInfo.Num(); i++)
	{
		if (!ItemsInfo[i].TextureName.IsEmpty())
			ItemsInfo[i].Texture = UHKAssetManager::Get().GetItemTexture(ItemsInfo[i].TextureName);
	}

	SendChangedStoreInformationToClients();
}





