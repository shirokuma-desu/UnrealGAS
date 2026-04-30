// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

//Macro
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName,PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){}

	FGameplayEffectContextHandle EffectContextHandle;
	
	UPROPERTY()
	AActor* TargetAvatarActor = nullptr;
	UPROPERTY()
	AController* TargetController = nullptr;
	UPROPERTY()
	ACharacter* TargetCharacter = nullptr ;
	UPROPERTY()
	UAbilitySystemComponent* TargetASC = nullptr;

	UPROPERTY()
	AActor* SourceAvatarActor = nullptr;
	UPROPERTY()
	AController* SourceController = nullptr;
	UPROPERTY()
	ACharacter* SourceCharacter = nullptr ;
	UPROPERTY()
	UAbilitySystemComponent* SourceASC = nullptr;

};

//declare a template
//type def is specific to FGameplayAttribute() signature, but TStaticFunPtr is generic to any signature chosen
//typedef TBaseStaticDelegateInstance<FGameplayAttribute(),FDefaultDelegateUserPolicy> ::FFuncPtr FAttributeFunc;
template<class T>
using TStaticFunc = TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy> ::FFuncPtr;

UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UAuraAttributeSet();
	//must override to replicate any var
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	TMap<FGameplayTag,TStaticFunc<FGameplayAttribute()>> TagToAttribute;
	
	//Primary Attribute declare
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Strength,Category="Primary Attribute");
	FGameplayAttributeData Strength;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Intelligence,Category="Primary Attribute");
	FGameplayAttributeData Intelligence;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Resilience,Category="Primary Attribute");
	FGameplayAttributeData Resilience;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Vigor,Category="Primary Attribute");
	FGameplayAttributeData Vigor;

	//Second Attribute
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Armor,Category="Second Attribute");
	FGameplayAttributeData Armor;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_ArmorPen,Category="Second Attribute");
	FGameplayAttributeData ArmorPen;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_BlockChance,Category="Second Attribute");
	FGameplayAttributeData BlockChance;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_CritHitChance,Category="Second Attribute");
	FGameplayAttributeData CritHitChance;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_CritDmg,Category="Second Attribute");
	FGameplayAttributeData CritDmg;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_CritResist,Category="Second Attribute");
	FGameplayAttributeData CritResist;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_HealthRegen,Category="Second Attribute");
	FGameplayAttributeData HealthRegen;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_ManaRegen,Category="Second Attribute");
	FGameplayAttributeData ManaRegen;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxHealth,Category="Second Attribute");
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_MaxMana,Category="Second Attribute");
	FGameplayAttributeData MaxMana;
	
	
	//Vital Attribute declare
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Health,Category = "Vital Attributes")
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing = OnRep_Mana, Category = "Mana Attributes")
	FGameplayAttributeData Mana;
	
	//Resistance Attribute 
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_FireResistance, Category="Resistance Attribute")
	FGameplayAttributeData FireResistance; 
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_ArcaneResistance, Category="Resistance Attribute")
	FGameplayAttributeData ArcaneResistance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_LightingResistance, Category="Resistance Attribute")
	FGameplayAttributeData LightingResistance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing= OnRep_PhysicalResistance, Category="Resistance Attribute")
	FGameplayAttributeData PhysicalResistance;
	
	//Meta Attribute
	UPROPERTY(BlueprintReadOnly,  Category = "Meta Attribute")
	FGameplayAttributeData IncommingDamage;
	
	UPROPERTY(BlueprintReadOnly,  Category = "Meta Attribute")
	FGameplayAttributeData IncomingXP;

	//Gameplay AttributeSet Accessors
	
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Health)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Mana)
	//primary attribute access
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Strength)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Intelligence)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Resilience)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Vigor)
	//second attribute access
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxHealth)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxMana)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Armor)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ArmorPen)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CritDmg)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CritHitChance)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CritResist)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,HealthRegen)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ManaRegen)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,BlockChance)
	//Resistance attribute access
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,FireResistance)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,PhysicalResistance)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ArcaneResistance)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,LightingResistance)
	
	//Meta Attribute access
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,IncommingDamage)
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,IncomingXP)
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const;
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength) const;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence) const;
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience) const;
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor) const;
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor) const;
	UFUNCTION()
	void OnRep_ArmorPen(const FGameplayAttributeData& OldArmorPen) const;
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance) const;
	UFUNCTION()
	void OnRep_CritHitChance(const FGameplayAttributeData& OldCritHitChance) const;
	UFUNCTION()
	void OnRep_CritDmg(const FGameplayAttributeData& OldCritDmg) const;
	UFUNCTION()
	void OnRep_CritResist(const FGameplayAttributeData& OldCritResist) const;
	UFUNCTION()
	void OnRep_HealthRegen(const FGameplayAttributeData& OldHealthRegen) const;
	UFUNCTION()
	void OnRep_ManaRegen(const FGameplayAttributeData& OldManaRegen) const;
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldFireResistance) const;
	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldArcaneResistance) const;
	UFUNCTION()
	void OnRep_LightingResistance(const FGameplayAttributeData& OldLightingResistance) const;
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldPhysicalResistance) const;

private:
	void SetEffectProperties(const FGameplayEffectModCallbackData Data, FEffectProperties& Props) const;
	
	void ShowFloatingText(const FEffectProperties& Props,float Damage, bool bBlockHit, bool bCriticalHit) const;
	
	void SendXP(const FEffectProperties& Props );
	
	void HandleIncomingDamage(const FEffectProperties& Props);
	void HandleIncomingXP(const FEffectProperties& Props);
	void Debuff(FEffectProperties& Props );
	
	bool bTopOffHealth = false;
	bool bTopOffMana = false;
};



