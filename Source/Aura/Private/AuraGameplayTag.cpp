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
	
	//input tag
	GameplayTag.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.InputTag_LMB"),
		FString("Input Tag for Left mouse button"));
	GameplayTag.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.InputTag_RMB"),
		FString("Input Tag for right mouse button"));
	GameplayTag.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.InputTag_1"),
		FString("Input tag for 1 key"));
	GameplayTag.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.InputTag_2"),
		FString("Input tag for 2 key"));
	GameplayTag.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.InputTag_3"),
		FString("Input tag for 3 key"));
	GameplayTag.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.InputTag_4"),
		FString("Input tag for 4 key"));
	GameplayTag.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive_1"),
		FString("Input Tag For Passive 1"));
	GameplayTag.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.Passive_2"),
		FString("Input Tag For Passive 2"));
	
	
	GameplayTag.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"),
		FString("Damage"));
	
	//Damage Types	
	
	GameplayTag.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"),
		FString("Fire Damage Type"));
	GameplayTag.Damage_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lighting"),
		FString("Lighting Damage Type"));
	GameplayTag.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Arcane"),
		FString("Arcane Damage Type"));
	GameplayTag.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"),
		FString("Physical Damage Type"));
	
	//Resistance 
	GameplayTag.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"),
		FString("Fire Resistance"));
	GameplayTag.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Arcane"),
		FString("Arcane Resistance"));
	GameplayTag.Attributes_Resistance_Lighting = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lighting"),
		FString("Lighting Resistance"));
	GameplayTag.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"),
		FString("Physical Resistance"));
	
	
	
	//Combat Socket
	GameplayTag.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Weapon"),
	FString("Weapon"));
	GameplayTag.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.LeftHand"),
		FString(" Left Hand"));
	GameplayTag.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.RightHand"),
		FString(" Right Hand"));
	GameplayTag.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("CombatSocket.Tail"),
		FString(" Tail"));
	
	//Abilities 
	GameplayTag.Ability_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Attack"),
		FString("Attack Ability Task"));
	GameplayTag.Ability_Summon= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Summon"),
		FString("Summon Ability Task"));
	GameplayTag.Ability_None= UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.None"),
		FString("No Ability "));
	
	
	GameplayTag.Ability_Fire_FireBolt =	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Fire.FireBolt"),
		FString("FireBolt Ability Task"));
	GameplayTag.Ability_Lighting_Electrocute =	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Lighting.Electrocute"),
		FString("Electrocute Ability Task"));
	
	
	GameplayTag.Ability_HitReact =	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.HitReact"),
		FString("Hit React Ability"));
	
	GameplayTag.Ability_Status_Eligible =	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Eligible"),
		FString("Eligible Status"));
	GameplayTag.Ability_Status_Equipped =	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Equipped"),
		FString("Equipped Status"));
	GameplayTag.Ability_Status_Lock =	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Lock"),
		FString("Lock Status"));
	GameplayTag.Ability_Status_Unlocked =	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Status.Unlock"),
		FString("Unlock Status"));
	
	GameplayTag.Ability_Type_Offensive =	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Offensive"),
		FString("Type Offensive"));
	GameplayTag.Ability_Type_Passive =	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.Passive"),
		FString("Type Passive"));
	GameplayTag.Ability_Type_None =	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Abilities.Type.None"),
		FString("Type None"));
	
	//CooldownTag
	GameplayTag.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Cooldown.Fire.FireBolt"),
		FString("Cooldown FireBolt"));
	
	// Montage Tags
	GameplayTag.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.1"),
	FString("Attack 1"));
	GameplayTag.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.2"),
	FString("Attack 2"));
	GameplayTag.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.3"),
	FString("Attack 3"));
	GameplayTag.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Montage.Attack.4"),
	FString("Attack 4"));
	
	//Meta Attributes
	GameplayTag.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Meta.IncomingXP"),
		FString("Incoming XP Meta Attributes"));
	
	//Debuff
	GameplayTag.Debuff_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Arcane"),
		FString("Debuff for Arcane"));
	GameplayTag.Debuff_Burn = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Burn"),
		FString("Debuff for burn"));
	GameplayTag.Debuff_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Physical"),
		FString("Debuff Physical"));
	GameplayTag.Debuff_Stun = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Stun"),
		FString("Debuff Stun"));
	
	//Debuff Stats 
	GameplayTag.Debuff_Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Damage"),
		FString("Debuff Damage"));
	GameplayTag.Debuff_Chance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Chance"),
		FString("Debuff Chance"));
	GameplayTag.Debuff_Duration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Debuff.Duration"),
		FString("Debuff Duration"));
	GameplayTag.Debuff_Frequency = UGameplayTagsManager::Get().AddNativeGameplayTag("Debuff.Frequency",
		FString("Debuff Frequency"));
	
	
	GameplayTag.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(
		FName("Effects.HitReact"),
		FString("Tag granted when Hit Reacting")
		);
	
	
	// Map of damage types  to Resistance
	GameplayTag.DamageTypesToResistance.Add(GameplayTag.Damage_Fire,GameplayTag.Attributes_Resistance_Fire);
	GameplayTag.DamageTypesToResistance.Add(GameplayTag.Damage_Lighting,GameplayTag.Attributes_Resistance_Lighting);
	GameplayTag.DamageTypesToResistance.Add(GameplayTag.Damage_Physical,GameplayTag.Attributes_Resistance_Physical);
	GameplayTag.DamageTypesToResistance.Add(GameplayTag.Damage_Arcane,GameplayTag.Attributes_Resistance_Arcane);
	
	
	// Map of damage type to debuff
	
	GameplayTag.DamageTypesToDebuff.Add(GameplayTag.Damage_Fire,GameplayTag.Debuff_Burn);
	GameplayTag.DamageTypesToDebuff.Add(GameplayTag.Damage_Lighting,GameplayTag.Debuff_Stun);
	GameplayTag.DamageTypesToDebuff.Add(GameplayTag.Damage_Physical,GameplayTag.Debuff_Physical);
	GameplayTag.DamageTypesToDebuff.Add(GameplayTag.Damage_Arcane,GameplayTag.Debuff_Arcane);
	
}
	
	

