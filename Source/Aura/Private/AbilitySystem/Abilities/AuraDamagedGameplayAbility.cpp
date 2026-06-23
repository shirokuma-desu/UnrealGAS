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
	DamageEffectParams.KnockbackForceMagnitude = KnockbackForceMagnitude;
	DamageEffectParams.KnockbackChance = KnockBackChance;
	if (IsValid(TargetActor))
	{
		FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;
		const FVector ToTarget = Rotation.Vector();
		DamageEffectParams.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		if (const bool bKnockback = FMath::RandRange(1,100) <= DamageEffectParams.KnockbackChance)
		{
			DamageEffectParams.KnockbackForce = ToTarget * KnockbackForceMagnitude;
		}
	}
	return DamageEffectParams;
}

float UAuraDamagedGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}

FTaggedMontage UAuraDamagedGameplayAbility::GetRandomTagMontageFromArr(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if (TaggedMontages.IsEmpty()) return FTaggedMontage{};
	
	const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
	return  TaggedMontages[Selection];
}


