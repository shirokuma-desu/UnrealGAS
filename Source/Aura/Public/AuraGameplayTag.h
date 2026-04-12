// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Aura GameplayTag
 * Singleton containing native gameplayTag
 */
struct FAuraGameplayTag
{
public:
	static const FAuraGameplayTag& Get(){return GameplayTag; };
	static void InitializeNativeGameplayTag();
	
	
	//Native Primary Attributes
	FGameplayTag Attributes_Primary_Strength;
	FGameplayTag Attributes_Primary_Intelligence;
	FGameplayTag Attributes_Primary_Resilience;
	FGameplayTag Attributes_Primary_Vigor;
	
	//Native Secondary Attributes
	FGameplayTag Attributes_Secondary_Armor;
	FGameplayTag Attributes_Secondary_ArmorPen;
	FGameplayTag Attributes_Secondary_BlockChance;
	FGameplayTag Attributes_Secondary_CritHitChance;
	FGameplayTag Attributes_Secondary_CritResist;
	FGameplayTag Attributes_Secondary_CritDmg;
	FGameplayTag Attributes_Secondary_HealthRegen;
	FGameplayTag Attributes_Secondary_ManaRegen;
	FGameplayTag Attributes_Secondary_MaxHealth;
	FGameplayTag Attributes_Secondary_MaxMana;
	
	FGameplayTag Attributes_Resistance_Fire;
	FGameplayTag Attributes_Resistance_Lighting;
	FGameplayTag Attributes_Resistance_Arcane;
	FGameplayTag Attributes_Resistance_Physical;
	
	FGameplayTag Attributes_Meta_IncomingXP;
	
	//Input
	FGameplayTag InputTag_LMB; 
	FGameplayTag InputTag_RMB; 
	FGameplayTag InputTag_1; 
	FGameplayTag InputTag_2; 
	FGameplayTag InputTag_3; 
	FGameplayTag InputTag_4; 
	FGameplayTag InputTag_Passive_1; 
	FGameplayTag InputTag_Passive_2; 
	
	
	FGameplayTag Damage;
	FGameplayTag HitReact;
	FGameplayTag Damage_Fire;
	FGameplayTag Damage_Lighting;
	FGameplayTag Damage_Arcane;
	FGameplayTag Damage_Physical;
	
	FGameplayTag Ability_Attack;
	FGameplayTag Ability_Summon;
	
	FGameplayTag Ability_Status_Lock;
	FGameplayTag Ability_Status_Eligible;
	FGameplayTag Ability_Status_Unlocked;
	FGameplayTag Ability_Status_Equipped;
	
	FGameplayTag Ability_Type_Offensive;
	FGameplayTag Ability_Type_Passive;
	FGameplayTag Ability_Type_None;
	
	FGameplayTag Ability_HitReact;
	
	FGameplayTag Ability_Fire_FireBolt;
	
	
	FGameplayTag Cooldown_Fire_FireBolt;
	
	FGameplayTag CombatSocket_Weapon;
	FGameplayTag CombatSocket_RightHand;
	FGameplayTag CombatSocket_LeftHand;
	FGameplayTag CombatSocket_Tail;
	
	
	FGameplayTag Montage_Attack_1;
	FGameplayTag Montage_Attack_2;
	FGameplayTag Montage_Attack_3;
	FGameplayTag Montage_Attack_4;
	
	
	
	TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;
	
	
private:
	static FAuraGameplayTag GameplayTag;
};


