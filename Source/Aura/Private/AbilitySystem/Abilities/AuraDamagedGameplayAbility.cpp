// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamagedGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityType.h"

void UAuraDamagedGameplayAbility::CauseDamage(AActor* TargetActor)
{
	auto DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass,1);
	
	float DamageMagnitude = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,DamageType,DamageMagnitude);
	
	GetAbilitySystemComponentFromActorInfo()->
	ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}

FDamageEffectParams UAuraDamagedGameplayAbility::MakeDamageEffectParamsFromClassDefault(AActor* TargetActor) const
{
	FDamageEffectParams DamageEffectParams;
	DamageEffectParams.WorldContextObject = GetAvatarActorFromActorInfo();
	DamageEffectParams.DamagedGameplayEffectClass = DamageEffectClass;
	DamageEffectParams.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	DamageEffectParams.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	DamageEffectParams.AbilityLevel = GetAbilityLevel();
	DamageEffectParams.DamagedType = DamageType;
	DamageEffectParams.DebuffChance = DebuffChance;
	DamageEffectParams.DebuffDuration = DebuffDuration;
	DamageEffectParams.DebuffFrequency = DebuffFrequency;
	DamageEffectParams.DebuffDamage = DebuffDamage;
	DamageEffectParams.DeathImpulseMagnitude = DeathImpulseMagnitude;
	return DamageEffectParams;
}

FTaggedMontage UAuraDamagedGameplayAbility::GetRandomTagMontageFromArr(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.IsEmpty()) return FTaggedMontage{};
	
	const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
	return  TaggedMontages[Selection];
}


