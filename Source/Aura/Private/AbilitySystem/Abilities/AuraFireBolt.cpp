// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "Kismet/KismetSystemLibrary.h"

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
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectilesSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectilesSpread /2.f, FVector::UpVector);
	NumberOfProjectiles = FMath::Min(MaxNumProjectiles,GetAbilityLevel());
	
	if (NumberOfProjectiles > 1)
	{
		const float DeltaSpread = ProjectilesSpread / (NumberOfProjectiles -1);
		for (int32 i = 0; i < NumberOfProjectiles; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			const FVector Start = SocketLocation + FVector(0,0,10);
			UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
				Start, Start + Direction * 75.f,1, 
				FLinearColor::Red, 120, 1);
		}
	}
	else
	{
		const FVector Start = SocketLocation + FVector(0,0,5);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),
				Start, Start + Forward * 75.f,1, 
				FLinearColor::Red, 120, 1);
		//Single Projectile
	}
	
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),SocketLocation, SocketLocation + Forward * 100,5, FLinearColor::Green, 120, 2);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),SocketLocation, LeftOfSpread + Rotation.Vector() * 100,5, FLinearColor::Gray, 120, 1);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(),SocketLocation, RightOfSpread + Rotation.Vector() * 100,5, FLinearColor::Gray, 120, 1);
}
