// Fill out your copyright notice in the Description page of Project Settings.


#include "HKAssetManager.h"
#include "HKGameplayTags.h"
#include "AbilitySystemGlobals.h"

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
}
