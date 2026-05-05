// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraCharacterBase.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	
	BurnDebuffComponent = CreateDefaultSubobject<UDebuffNiagaraComponent>("BurnDebuffComponent");
	BurnDebuffComponent->SetupAttachment(GetRootComponent());
	BurnDebuffComponent->DebuffTag = FAuraGameplayTag::Get().Debuff_Burn;
	
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	WeaponMeshComponent->SetupAttachment(GetMesh(),FName("WeaponHandSocket"));
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(false);
	
	ECharacterClass = ECharacterClass::Elementalist;
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	WeaponMeshComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	MC_HandleDeath();
}

void AAuraCharacterBase::MC_HandleDeath_Implementation()
{
	WeaponMeshComponent->SetSimulatePhysics(true);
	WeaponMeshComponent->SetEnableGravity(true);
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
	Dissolve();
	
	UGameplayStatics::PlaySoundAtLocation(this,DeadSound,GetActorLocation(),GetActorRotation());
	
	bDead = true;
	OnDead.Broadcast(this);
}

// Called when the game starts or when spawned
void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag)
{
	const FAuraGameplayTag& GameplayTags = FAuraGameplayTag::Get();
	
	if (MontageTag.MatchesTagExact(FAuraGameplayTag::Get().CombatSocket_Weapon) && IsValid(WeaponMeshComponent))
	{
		return WeaponMeshComponent->GetSocketLocation(WeaponTipSocketName);	
	}
	if (MontageTag.MatchesTagExact(FAuraGameplayTag::Get().CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocket);	
	}
	if (MontageTag.MatchesTagExact(FAuraGameplayTag::Get().CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocket);	
	}
	if (MontageTag.MatchesTagExact(FAuraGameplayTag::Get().CombatSocket_Tail))
	{
		return  GetMesh()->GetSocketLocation(TailSocket);
	}
	return FVector();
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bDead;
}


AActor* AAuraCharacterBase::GetAvatarActor_Implementation() 
{
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAllAttackMontage_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation()
{
	return BloodEffect;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (auto Element : AttackMontages)
	{
		if (Element.MontageTag == MontageTag)
		{
			return Element;
		}
	}
	return  FTaggedMontage();
}

int32 AAuraCharacterBase::GetMinionCount_Implementation()
{
	return MinionCount;
}

void AAuraCharacterBase::IncrementMinionCount_Implementation(int32 Amount)
{
	MinionCount+= Amount;
}

FOnASCRegistered AAuraCharacterBase::GetOnASCRegisteredDelegate()
{
	return OnASCRegistered;
}

FOnDead AAuraCharacterBase::GetOnDeadDelegate()
{
	return  OnDead;
}

ECharacterClass AAuraCharacterBase::GetCharacterClass_Implementation()
{
	return ECharacterClass;
}


void AAuraCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle GameplayEffectContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	GameplayEffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle GameplayEffectSpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass,level,GameplayEffectContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*GameplayEffectSpecHandle.Data.Get(),GetAbilitySystemComponent());
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes,1);
	ApplyEffectToSelf(DefaultSecondaryPrimaryAttributes,1);
	ApplyEffectToSelf(DefaultVitalAttributes,1.0f);
}

void AAuraCharacterBase::AddCharacterAbilities() const
{
	UAuraAbilitySystemComponent* AuraASC =  CastChecked<UAuraAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority()) return;
	AuraASC->AddCharacterAbility(StartupAbilities);
	AuraASC->AddCharacterPassiveAbility(StartupPassiveAbilities);
	
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(DissolveMaterialInstanceMesh))
	{
		UMaterialInstanceDynamic* DynamicMatInst = UMaterialInstanceDynamic::
		Create(DissolveMaterialInstanceMesh, this);
		GetMesh()->SetMaterial(0,DynamicMatInst);
		StartDissolveTimelineMesh(DynamicMatInst);
	}
	if (IsValid(DissolveMaterialInstanceWeapon))
	{
		UMaterialInstanceDynamic* DynamicMatInstWeapon = UMaterialInstanceDynamic::
		Create(DissolveMaterialInstanceWeapon,this);
		WeaponMeshComponent->SetMaterial(0,DynamicMatInstWeapon);
		StartDissolveTimelineWeapon(DynamicMatInstWeapon);
	}
}

// Called to bind functionality to input
void AAuraCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

