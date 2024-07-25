// Copyright Druid Mechanics

#include "Item/Inventory.h"
#include "Lobby/HKUILobbyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WidgetController/InventoryWidgetController.h"


void UInventory::ReSettingItems(const TArray<int> Ids, const TArray<int> Count)
{
	ConsumableItems.Empty();
	EquipmentItems.Empty();
	Characters.Empty();
	AddItemsToArray(Ids, Count);
}

void UInventory::AddItemsToArray(const TArray<FUserItem> Items)
{
	for (FUserItem Item : Items)
	{
		AddItem(Item);
	}
}

void UInventory::AddItemsToArray(const TArray<int> Ids,const TArray<int> Count)
{
	for (int i = 0; i < Ids.Num(); i++)
	{
		AddItem(Ids[i], Count[i]);
	}
}

void UInventory::AddItem(const int Id, const int Count, int UniqueID)
{
	FUserItem NewItem;
	NewItem.Id = Id;
	NewItem.UniqueId = UniqueID;
	NewItem.Count = Count;

	AddItem(NewItem);
}

void UInventory::AddItem(const FUserItem Item)
{
	UE_LOG(LogTemp, Log, TEXT("%d : %d"), Item.Id, Item.Count);

	FItemInfomation ConsumableItemInfo = ConsumableItemsInfo->FindItemInfoForItemID(Item.Id);
	if (ConsumableItemInfo.Id != -1)
	{
		AddConsumableItem(Item);
		SendChangedInventoryInformationToClients();
		return;
	}

	FItemInfomation EquipmentItemInfo = EquipmentItemsInfo->FindItemInfoForItemID(Item.Id);
	if (EquipmentItemInfo.Id != -1)
	{
		AddEquipmentItem(Item);
		SendChangedInventoryInformationToClients();
		return;
	}

	FItemInfomation CharacterItemInfo = CharacterItemsInfo->FindItemInfoForItemID(Item.Id);
	if (CharacterItemInfo.Id != -1)
	{
		AddCharacter(Item);
		SendChangedInventoryInformationToClients();
		return;
	}
}

void UInventory::AddConsumableItem(const FUserItem Item)
{
	FUserItem* UserItem = ConsumableItems.Find(Item.Id);
	if (UserItem == nullptr)
	{
		FItemInfomation ItemInfo = ConsumableItemsInfo->FindItemInfoForItemID(Item.Id);
		if (ItemInfo.Id == -1)
		{
			return;
		}

		FUserItem NewUserItem;
		NewUserItem.Id = Item.Id;
		NewUserItem.Count = Item.Count;
		NewUserItem.ItemInfo = ItemInfo;
		ConsumableItems.Add(Item.Id, NewUserItem);
	}
	else
	{
		UserItem->Count = UserItem->Count + Item.Count;
	}
}

void UInventory::AddEquipmentItem(const FUserItem Item)
{
	FItemInfomation ItemInfo = EquipmentItemsInfo->FindItemInfoForItemID(Item.Id);
	if (ItemInfo.Id == -1)
	{
		return;
	}

	FUserItem NewUserItem;
	NewUserItem.Id = Item.Id;
	NewUserItem.UniqueId = Item.UniqueId;
	NewUserItem.Count = Item.Count;
	NewUserItem.ItemInfo = ItemInfo;

	//장비는 Unique ID로 저장 (장비는 동일 장비를 여러개 가질 수 있음)
	EquipmentItems.Add(Item.UniqueId, NewUserItem);
}

void UInventory::AddCharacter(const FUserItem Item)
{
	for(FUserItem Character : Characters)
	{
		if (Character.Id == Item.Id)
		{
			return;
		}
	}

	FItemInfomation ItemInfo = CharacterItemsInfo->FindItemInfoForItemID(Item.Id);
	if (ItemInfo.Id == -1)
	{
		return;
	}

	FUserItem NewUserItem;
	NewUserItem.Id = Item.Id;
	NewUserItem.Count = Item.Count;
	NewUserItem.ItemInfo = ItemInfo;
	Characters.Add(NewUserItem);

}

void UInventory::SendChangedInventoryInformationToClients()
{
	//Lobby 에서만 동기화를 위해 보내줍니다.
	AHKUILobbyPlayerController* LocalClientPlayerController = Cast<AHKUILobbyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (!IsValid(LocalClientPlayerController))
	{
		return;
	}

	if (InventoryWidgetController == nullptr)
	{
		InventoryWidgetController = NewObject<UInventoryWidgetController>(this, UInventoryWidgetController::StaticClass());
	}
	LocalClientPlayerController->SetInventoryWidgetController(InventoryWidgetController);

	FInventoryWidgetControllerParams Params;
	Params.Characters = Characters;
	Params.ConsumableItems = ConsumableItems;
	InventoryWidgetController->SetWidgetControllerParams(Params);
}
