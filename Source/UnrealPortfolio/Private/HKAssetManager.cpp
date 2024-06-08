// Fill out your copyright notice in the Description page of Project Settings.


#include "HKAssetManager.h"
#include "HKGameplayTags.h"
#include "Engine/Texture2D.h"
#include "AbilitySystemGlobals.h"
#include "UnrealPortfolio/UnrealPortfolio.h"

UHKAssetManager& UHKAssetManager::Get()
{
	check(GEngine);

	UHKAssetManager* HKAssetManager = Cast<UHKAssetManager>(GEngine->AssetManager);
	return *HKAssetManager;
}

void UHKAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FHKGameplayTags::InitializeNativeGameplayTags();

	//This is required to use Target Data!
	UAbilitySystemGlobals::Get().InitGlobalData();

	LoadItemTexture();
}


UTexture2D* UHKAssetManager::GetItemTexture(FString TextureName)
{
	return ItemsTexture.Find(TextureName)->Get();
}

void UHKAssetManager::LoadItemTexture()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName("AssetRegistry"));
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();

	TArray<FString> PathsToScan;
	PathsToScan.Add(DefaultItemsImagePath);
	AssetRegistry.ScanPathsSynchronous(PathsToScan);

	TArray<FAssetData> MeshAssetList;
	AssetRegistry.GetAssetsByPath(FName(DefaultItemsImagePath), MeshAssetList);
	for (FAssetData AssetData : MeshAssetList)
	{
		UTexture2D* TextData = Cast<UTexture2D>(AssetData.GetAsset());
		if (TextData)
		{
			ItemsTexture.Add(AssetData.AssetName.ToString(), TextData);
		}
		
	}
}
