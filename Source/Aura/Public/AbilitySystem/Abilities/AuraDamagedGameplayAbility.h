// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "Interfaces/CombatInterface.h"
#include "AuraDamagedGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraDamagedGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);
	
	
	protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category= "Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypesMap;
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTagMontageFromArr(const TArray<FTaggedMontage>& TaggedMontages) const;
	
	float GetDamageByDamageType(float InLevel,const FGameplayTag& DamageType);
};
