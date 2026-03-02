// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraDamagedGameplayAbility.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamagedGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
	UFUNCTION(BlueprintCallable,Category="Aura|ProjectileSpell")
	void SpawnProjectile(const FVector& TargetVector);
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class AAuraProjectile> ProjectileClass;
	
	
};
