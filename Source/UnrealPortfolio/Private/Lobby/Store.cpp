// Copyright Druid Mechanics


#include "Lobby/Store.h"
#include "HKDatabaseFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "HKAssetManager.h"


AStore::AStore()
{
	PrimaryActorTick.bCanEverTick = false;
	bAlwaysRelevant = true;
	bReplicates = true;
	NetUpdateFrequency = 10.f;

}

void AStore::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, ItemsInfo);
}

void AStore::InitializeSaleItems(TArray<FStoreItemDefaultInfo> Items)
{
	if (Items.Num() == 0)
	{
		return;
	}

	ItemsInfo = Items;
}

void AStore::OnRep_ItemInfo()
{
	for (int i = 0; i < ItemsInfo.Num(); i++)
	{
		if(!ItemsInfo[i].TextureName.IsEmpty())
			ItemsInfo[i].Texture = UHKAssetManager::Get().GetStoreItemTexture(ItemsInfo[i].TextureName);
	}
}





