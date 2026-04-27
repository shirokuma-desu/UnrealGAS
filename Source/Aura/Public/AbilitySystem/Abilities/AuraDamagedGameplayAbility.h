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
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, Category="Damage")
	FScalableFloat Damage;
	
	
	
	UFUNCTION(BlueprintPure)
	FTaggedMontage GetRandomTagMontageFromArr(const TArray<FTaggedMontage>& TaggedMontages) const;
	
};
