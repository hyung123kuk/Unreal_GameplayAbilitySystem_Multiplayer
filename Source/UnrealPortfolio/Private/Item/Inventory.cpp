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

void UInventory::UseItem(const int Id, const int UniqueID)
{
	FItemInfomation ConsumableItemInfo = ConsumableItemsInfo->FindItemInfoForItemID(Id);
	if (ConsumableItemInfo.Id != -1)
	{
		UseConsumableItem(Id);
		return;
	}

	FItemInfomation EquipmentItemInfo = EquipmentItemsInfo->FindItemInfoForItemID(Id);
	if (EquipmentItemInfo.Id != -1)
	{
		UseEquipmentItem(Id, UniqueID);
		return;
	}

}

void UInventory::RemoveItem(const int Id, int count, const int UniqueID)
{
	FItemInfomation ConsumableItemInfo = ConsumableItemsInfo->FindItemInfoForItemID(Id);
	if (ConsumableItemInfo.Id != -1)
	{
		RemoveConsumableItem(Id,count);
		SendChangedInventoryInformationToClients();
		return;
	}

	FItemInfomation EquipmentItemInfo = EquipmentItemsInfo->FindItemInfoForItemID(Id);
	if (EquipmentItemInfo.Id != -1)
	{
		RemoveEquipmentItem(Id, UniqueID);
		SendChangedInventoryInformationToClients();
		return;
	}

	FItemInfomation CharacterItemInfo = CharacterItemsInfo->FindItemInfoForItemID(Id);
	if (CharacterItemInfo.Id != -1)
	{
		RemoveCharacter(Id);
		SendChangedInventoryInformationToClients();
		return;
	}

}

FUserItem UInventory::FindItem(const int Id, const int UniqueId)
{
	FItemInfomation ConsumableItemInfo = ConsumableItemsInfo->FindItemInfoForItemID(Id);
	if (ConsumableItemInfo.Id != -1)
	{
		FUserItem* UserItem = ConsumableItems.Find(Id);
		if (UserItem != nullptr)
		{
			FUserItem NewUserItem = *UserItem;
			return NewUserItem;
		}

	}

	FItemInfomation EquipmentItemInfo = EquipmentItemsInfo->FindItemInfoForItemID(Id);
	if (EquipmentItemInfo.Id != -1)
	{
		FUserItem* UserItem = EquipmentItems.Find(UniqueId);
		if (UserItem != nullptr)
		{
			FUserItem NewUserItem = *UserItem;
			return NewUserItem;
		}
	}

	return FUserItem();
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

		FUserItem NewUserItem = MakeNewUserItem(Item, ItemInfo);

		ConsumableItems.Add(Item.Id, NewUserItem);
		NewItemDelegate.Broadcast(NewUserItem);
	}
	else
	{
		FItemInfomation ItemInfo = ConsumableItemsInfo->FindItemInfoForItemID(Item.Id);
		UserItem->Count = UserItem->Count + Item.Count;
		FUserItem NewUserItem = *UserItem;
		ChangeItemValueDelegate.Broadcast(NewUserItem);
	}

}

void UInventory::UseConsumableItem(const int Id)
{
	FUserItem* UserItem = ConsumableItems.Find(Id);
	if (UserItem != nullptr)
	{
		FUserItem NewUserItem = *UserItem;
		UseItemValueDelegate.Broadcast(NewUserItem);
	}
}

void UInventory::RemoveConsumableItem(const int Id, int count)
{
	FUserItem* UserItem = ConsumableItems.Find(Id);
	if (UserItem != nullptr)
	{
		UserItem->Count -= count;
		if (UserItem->Count <= 0)
		{
			RemoveItemValueDelegate.Broadcast(*UserItem);
			ConsumableItems.Remove(Id);
		}
		else
		{
			ChangeItemValueDelegate.Broadcast(*UserItem);
		}
	}
}

void UInventory::AddEquipmentItem(const FUserItem Item)
{
	FItemInfomation ItemInfo = EquipmentItemsInfo->FindItemInfoForItemID(Item.Id);
	if (ItemInfo.Id == -1)
	{
		return;
	}

	FUserItem NewUserItem = MakeNewUserItem(Item, ItemInfo);

	//장비는 Unique ID로 저장 (장비는 동일 장비를 여러개 가질 수 있음)
	EquipmentItems.Add(Item.UniqueId, NewUserItem);
	NewItemDelegate.Broadcast(NewUserItem);
}

void UInventory::UseEquipmentItem(const int Id, const int UniqueId)
{
	FUserItem* UserItem = EquipmentItems.Find(UniqueId);
	if (UserItem != nullptr)
	{
		FUserItem NewUserItem = *UserItem;
		UseItemValueDelegate.Broadcast(NewUserItem);
	}
}

void UInventory::RemoveEquipmentItem(const int Id, const int UniqueId)
{
	FUserItem* UserItem = EquipmentItems.Find(UniqueId);
	if (UserItem != nullptr)
	{
		RemoveItemValueDelegate.Broadcast(*UserItem);
		EquipmentItems.Remove(UniqueId);
	}
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

	FUserItem NewUserItem = MakeNewUserItem(Item, ItemInfo);
	Characters.Add(NewUserItem);

}

void UInventory::RemoveCharacter(const int Id)
{
	FUserItem RemoveItem;
	for (FUserItem Character : Characters)
	{
		if (Character.Id == Id)
		{
			RemoveItem = Character;
			break;
		}
	}

	RemoveItemValueDelegate.Broadcast(RemoveItem);
	Characters.Remove(RemoveItem);
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

FUserItem UInventory::MakeNewUserItem(const FUserItem UserInfo, const FItemInfomation ItemInfo)
{
	FUserItem NewUserItem;
	NewUserItem.Id = UserInfo.Id;
	NewUserItem.Count = UserInfo.Count;
	NewUserItem.UniqueId = UserInfo.UniqueId;
	NewUserItem.ItemInfo = ItemInfo;
	NewUserItem.Name = ItemInfo.Name;
	NewUserItem.Explanation = ItemInfo.Explanation;
	NewUserItem.SlotTexture = ItemInfo.Texture;
	NewUserItem.SlotInformation = ESlotContainInformation::Item;

	return NewUserItem;
}
