// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"
#include "AbilitySystem/AuraAbilitySystemBPLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UAuraFireBolt::GetDescription(int32 Level)
{
	
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	
	const float ManaCost =FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCoolDown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>FIRE BOLT</>\n\n"
			//Details
			"<Small>Level: </>"
			"<Level>%d</>\n"
			"<Small>Mana Cost: </>"
			"<ManaCost>%.1f</>\n"
			"<Small>Cooldown: </>"
			"<Cooldown>%.1f</>\n"
			//Description
			"<Default>Launch a bolt of fire,exploding on impact and dealing:</>"
			"<Damage>%d</><Default> fire damage with a chance to burn</>"
			
			),Level,ManaCost,CoolDown,ScaledDamage);
	}
	else
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>FIRE BOLT</>\n\n"
			//Details
			"<Small>Level: </>"
			"<Level>%d</>\n"
			"<Small>Mana Cost: </>"
			"<ManaCost>%.1f</>\n"
			"<Small>Cooldown: </>"
			"<Cooldown>%.1f</>\n"
			//Description
			"<Default>Launch %d bolts of fire,exploding on impact and dealing:</>"
			"<Damage>%d</><Default> fire damage with a chance to burn</>"
			
			),Level,ManaCost,CoolDown,FMath::Min(Level,NumberOfProjectiles),ScaledDamage);
		
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 Level)
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(Level);
	const float ManaCost =FMath::Abs(GetManaCost(Level));
	const float CoolDown = GetCoolDown(Level);
	return FString::Printf(TEXT(
			//Title
			"<Title>NEXT LEVEL: </>\n\n"
			//Details
			"<Small>Level: </>"
			"<Level>%d</>\n"
			"<Small>Mana Cost: </>"
			"<ManaCost>%.1f</>\n"
			"<Small>Cooldown: </>"
			"<Cooldown>%.1f</>\n"
			//Description
			"<Default>Launch %d bolts of fire,exploding on impact and dealing:</>"
			"<Damage>%d</><Default> fire damage with a chance to burn</>"
			
			),Level,ManaCost,CoolDown,FMath::Min(Level,NumberOfProjectiles),ScaledDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer) return;
	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),SocketTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	
	const FVector Forward = Rotation.Vector();
	const int32 EffectiveNumProjectiles = FMath::Min(NumberOfProjectiles,GetAbilityLevel());
	
	TArray<FRotator> Rotators = UAuraAbilitySystemBPLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectilesSpread, EffectiveNumProjectiles);
	
	
	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass, SpawnTransform,GetOwningActorFromActorInfo(),Cast<APawn>(GetOwningActorFromActorInfo()), 
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefault();
		if (HomingTarget->Implements<UCombatInterface>() && HomingTarget)
		{
			Projectile->ProjectileMovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovementComponent->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::RandRange(HomingAccelMin,HomingAccelMax);
		Projectile->ProjectileMovementComponent->bIsHomingProjectile = true;
		
		Projectile->FinishSpawning(SpawnTransform);
	}
	
}
