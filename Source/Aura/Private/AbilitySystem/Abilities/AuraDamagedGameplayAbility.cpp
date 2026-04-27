// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamagedGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

void UAuraDamagedGameplayAbility::CauseDamage(AActor* TargetActor)
{
	auto DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass,1);
	
	float DamageMagnitude = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle,DamageType,DamageMagnitude);
	
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


