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
	
	//Input
	FGameplayTag InputTag_LMB; 
	FGameplayTag InputTag_RMB; 
	FGameplayTag InputTag_1; 
	FGameplayTag InputTag_2; 
	FGameplayTag InputTag_3; 
	FGameplayTag InputTag_4; 
	
	FGameplayTag Damage;
	FGameplayTag HitReact;
	
protected:
	
private:
	static FAuraGameplayTag GameplayTag;
};


