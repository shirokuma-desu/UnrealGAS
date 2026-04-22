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

FTaggedMontage UAuraDamagedGameplayAbility::GetRandomTagMontageFromArr(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.IsEmpty()) return FTaggedMontage{};
	
	const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
	return  TaggedMontages[Selection];
}

float UAuraDamagedGameplayAbility::GetDamageByDamageType(float InLevel, const FGameplayTag& DamageType)
{
	checkf(DamageTypesMap.Contains(DamageType), TEXT("Gameplay Ability [%s] is not contain damage type [%s]"),*GetNameSafe(this),*DamageType.ToString())
	return DamageTypesMap[DamageType].GetValueAtLevel(InLevel);
}
