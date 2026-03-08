// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamagedGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamagedGameplayAbility::CauseDamage(AActor* TargetActor)
{
	auto DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass,1);
	for (auto Pair : DamageTypesMap)
	{
		float DamageMagnitude = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,Pair.Key,DamageMagnitude);
	}
	
	GetAbilitySystemComponentFromActorInfo()->
	ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(),
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor));
	
}
