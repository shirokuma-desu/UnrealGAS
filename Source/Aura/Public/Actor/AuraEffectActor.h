// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class USphereComponent;

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
	void ApplyGameplayEffectToTarget(AActor* ATarget, TSubclassOf<UGameplayEffect> EGameplayEffectClass);
	
	UPROPERTY(EditAnywhere, Category = "Applied Effect",BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InstanceGameplayEffectClass;
};
