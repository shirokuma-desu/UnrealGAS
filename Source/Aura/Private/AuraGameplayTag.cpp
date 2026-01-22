// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTag.h"

#include "GameplayTagsManager.h"


FAuraGameplayTag FAuraGameplayTag::GameplayTag;

void FAuraGameplayTag::InitializeNativeGameplayTag()
{
	GameplayTag.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"),
		FString("Increases physical damage"));
	GameplayTag.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"),
		FString("Increases magical damage"));
	GameplayTag.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"),
		FString("Increases Armor and Armor Penetration"));
	GameplayTag.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vigor"),
		FString("Increases Health"));
	
	GameplayTag.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), 
		FString("Reduces Damage Taken, improves block chance"));
	GameplayTag.Attributes_Secondary_ArmorPen = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmorPen"),
		FString("Ignores Percentage of enemy Armor, increases Critical Hit Chance"));
	GameplayTag.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"),
		FString("Chance to cut incoming damage in half"));
	GameplayTag.Attributes_Secondary_CritHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CritHitChance"),
		FString("Chance to double damage plus critical hit bonus"));
	GameplayTag.Attributes_Secondary_CritResist = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CritResist"),
		FString("Reduces Critical Hit Chance of attacking enemies"));
	GameplayTag.Attributes_Secondary_CritDmg = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CritDmg"),
		FString("Bonus damage added when a critical hit is scored"));
	GameplayTag.Attributes_Secondary_HealthRegen = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegen"),
		FString("Amount of Health regenerated every 1 second"));
	GameplayTag.Attributes_Secondary_ManaRegen = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegen"),
		FString("Amount of Mana regenerated every 1 second"));
	GameplayTag.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"),
		FString("Maximum amount of Health obtainable"));
	GameplayTag.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"),
		FString("Maximum amount of Mana obtainable"));
	
}

