// Copyright Druid Mechanics


#include "AbilitySystem/HKAbilitySystemLibrary.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Player/HKPlayerState.h"
#include "HKAbilityTypes.h"
#include "Game/HKGameMode.h"
#include "Interaction/CombatInterface.h"
#include "Interaction/MouseTargetActorInterface.h"
#include "AbilitySystemComponent.h"

UCharacterClassInfo* UHKAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AHKGameMode* HKGameMode = Cast<AHKGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (HKGameMode == nullptr) return nullptr;
	return  HKGameMode->CharacterClassInfo;
}

void UHKAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{

		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass);
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

void UHKAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
	PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
	SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
	VitalAttributesContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
}

void UHKAbilitySystemLibrary::GetLivePlayersWithinRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			ICombatInterface* CombatActor = Cast<ICombatInterface>(Overlap.GetActor());
			if (CombatActor != nullptr && !CombatActor->IsDead())
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
	}
}

void UHKAbilitySystemLibrary::GetLiveOtherTeamActorsWithinRadius(const UObject* WorldContextObject,const FGameplayTag& TeamTag, TArray<AActor*>& OutOverlappingActors, float Radius, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;

	TArray<FOverlapResult> Overlaps;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			ICombatInterface* CombatActor = Cast<ICombatInterface>(Overlap.GetActor());
			if (CombatActor != nullptr && !CombatActor->IsDead() && CombatActor->GetTeam() != TeamTag)
			{
				OutOverlappingActors.AddUnique(Overlap.GetActor());
			}
		}
	}
}

void UHKAbilitySystemLibrary::GetLiveOtherTeamActors(const UObject* WorldContextObject, const FGameplayTag& TeamTag, TArray<AActor*>& OutActors)
{
	QUICK_SCOPE_CYCLE_COUNTER(UGameplayStatics_GetAllActorsWithInterface);
	OutActors.Reset();

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;
			ICombatInterface* CombatActor = Cast<ICombatInterface>(Actor);
			if (CombatActor != nullptr && CombatActor->GetTeam() != TeamTag)
			{
				OutActors.Add(Actor);
			}
		}
	}
}

bool UHKAbilitySystemLibrary::IsMiss(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHKGameplayEffectContext* HKEffectContext = static_cast<const FHKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HKEffectContext->IsMiss();
	}
	return false;
}

bool UHKAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FHKGameplayEffectContext* HKEffectContext = static_cast<const FHKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return HKEffectContext->IsCriticalHit();
	}
	return false;
}

void UHKAbilitySystemLibrary::SetIsMiss(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsMiss)
{
	if (FHKGameplayEffectContext* HKEffectContext = static_cast<FHKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HKEffectContext->SetIsMiss(bInIsMiss);
	}
}

void UHKAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FHKGameplayEffectContext* HKEffectContext = static_cast<FHKGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		HKEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}

}