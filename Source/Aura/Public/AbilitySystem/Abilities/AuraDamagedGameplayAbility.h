// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraDamagedGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamagedGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
	protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category= "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypesMap;
};
