// Copyright Druid Mechanics


#include "Character/HKCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "AbilitySystem/HKAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HKGameplayTags.h"
#include "BlueprintGameplayTagLibrary.h"
#include "UnrealPortfolio/UnrealPortfolio.h"

AHKCharacterBase::AHKCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);


}

UAbilitySystemComponent* AHKCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHKCharacterBase::Die()
{
	Weapon->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MulticastHandleDeath();
}

bool AHKCharacterBase::IsDead() const
{
	return bDead;
}

void AHKCharacterBase::SetCombatTarget(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AHKCharacterBase::GetCombatTarget() const
{
	return CombatTarget;
}

TArray<FTaggedMontage> AHKCharacterBase::GetAttackMontages()
{
	return AttackMontage;
}

FVector AHKCharacterBase::GetCombatSocketLocation(const FGameplayTag& SocketTag, const FName& SocketName)
{
	const FHKGameplayTags& GameplayTags = FHKGameplayTags::Get();
	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_Character))
	{
		return GetMesh()->GetSocketLocation(SocketName);
	}

	if (SocketTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && IsValid(Weapon))
	{
		return Weapon->GetSocketLocation(SocketName);
	}

	return FVector();
}

const FGameplayTag& AHKCharacterBase::GetTeam() const
{
	return Team;
}

TArray<FTaggedMontage> AHKCharacterBase::GetActMontages()
{
	return ActMontage;
}

FUserItem AHKCharacterBase::GetEquipItem(FGameplayTag EquipTag)
{
	FUserItem* EquipItem = EquipmentItem.Find(EquipTag);
	return *EquipItem;
}

void AHKCharacterBase::UseItem(FUserItem Item)
{
	for (FGameplayTag TriggerTag : Item.ItemInfo.TriggerTags) 
	{
		OccurGameplayTags(TriggerTag);
	}

	for (TSubclassOf<UGameplayEffect> Effect : Item.ItemInfo.OccurEffects)
	{
		ApplyEffectToSelf(Effect,1.0f);
	}
}

void AHKCharacterBase::SetEquipItem(FUserItem NewItem)
{
	if (EquipmentItem.Contains(NewItem.ItemInfo.EquipmentTag))
	{
		FUserItem* UserItem = EquipmentItem.Find(NewItem.ItemInfo.EquipmentTag);
		if (UserItem->Id != -1)
		{
			RemoveCharacterAbilities(UserItem->ItemInfo.GiveAbilities);
			EquipmentItem.Remove(UserItem->ItemInfo.EquipmentTag);
		}
	}

	EquipmentItem.Add(NewItem.ItemInfo.EquipmentTag, NewItem);

	AddCharacterAbilities(NewItem.ItemInfo.GiveAbilities);
	UseItem(NewItem);
}

void AHKCharacterBase::MulticastHandleDeath_Implementation()
{
	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	
	Weapon->SetSimulatePhysics(true);
	Weapon->SetEnableGravity(true);
	Weapon->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bDead = true;
}

void AHKCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHKCharacterBase::InitAbilityActorInfo()
{
}

void AHKCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void AHKCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void AHKCharacterBase::AddCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities)
{
	UHKAbilitySystemComponent* HKASC = CastChecked<UHKAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	HKASC->AddCharacterAbilities(Abilities);
	
}

void AHKCharacterBase::RemoveCharacterAbilities(TArray<TSubclassOf<UGameplayAbility>> Abilities)
{
	UHKAbilitySystemComponent* HKASC = CastChecked<UHKAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;

	HKASC->RemoveAbilities(Abilities);
}

void AHKCharacterBase::OccurGameplayTags(FGameplayTag GameplayTags)
{
	FGameplayTagContainer TagContainer = UBlueprintGameplayTagLibrary::MakeGameplayTagContainerFromTag(GameplayTags);
	AbilitySystemComponent->TryActivateAbilitiesByTag(TagContainer);
}