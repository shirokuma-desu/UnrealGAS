// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

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