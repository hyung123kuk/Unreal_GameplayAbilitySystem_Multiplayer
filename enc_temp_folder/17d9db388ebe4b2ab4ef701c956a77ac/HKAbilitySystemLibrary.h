// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Character/CharacterClassInfo.h"
#include "HKAbilitySystemLibrary.generated.h"


/**
 * 
 */
UCLASS()
class UNREALPORTFOLIO_API UHKAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "HKAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "HKAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "HKAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "HKAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "HKAbilitySystemLibrary|GameplayMechanics")
	static void GetLiveOtherTeamActorsWithinRadius(const UObject* WorldContextObject, const FGameplayTag& TeamTag, TArray<AActor*>& OutOverlappingActors, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, Category = "HKAbilitySystemLibrary|GameplayMechanics")
	static void GetLiveOtherTeamActors(const UObject* WorldContextObject, const FGameplayTag& TeamTag, TArray<AActor*>& OutActors);

	UFUNCTION(BlueprintCallable, Category = "HKAbilitySystemLibrary|GameplayMechanics")
	static bool IsSameTeam(AActor* Actor, AActor* Actor2);

	UFUNCTION(BlueprintPure, Category = "HKAbilitySystemLibrary|GameplayEffects")
	static bool IsMiss(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category = "HKAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "HKAbilitySystemLibrary|GameplayEffects")
	static void SetIsMiss(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category = "HKAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

};
