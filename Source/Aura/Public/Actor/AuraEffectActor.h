// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;
class USphereComponent;

UENUM(BlueprintType)
enum EEffectApplicationPolicy : uint8
{
	E_ApplyEffectOnBeginOverlap,
	E_ApplyEffectOnEndOverlap,
	E_DoNotApplyEffect
};
UENUM(BlueprintType)
enum EEffectRemovalPolicy : uint8
{
	E_RemoveEffectOnEndOverlap,
	E_DoNotRemoveEffect
};


UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	//method
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void ApplyGameplayEffectToTarget(AActor* ATarget, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable)
	void OnBeginOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	void OnEndOverlap(AActor* TargetActor);

	//variable
	UPROPERTY(EditAnywhere, Category = "Applied Effect",BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InstanceGameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = "Applied Effect",BlueprintReadOnly,BlueprintReadOnly)
	TEnumAsByte<EEffectApplicationPolicy> InstanceEffectApplicationPolicy = E_DoNotApplyEffect;
	
	UPROPERTY(EditAnywhere, Category = "Applied Effect",BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = "Applied Effect",BlueprintReadOnly,BlueprintReadOnly)
	TEnumAsByte<EEffectApplicationPolicy> DurationEffectApplicationPolicy = E_DoNotApplyEffect;
	
	UPROPERTY(EditAnywhere,Category="Applied Effect",BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;

	UPROPERTY(EditAnywhere, Category = "Applied Effect",BlueprintReadOnly,BlueprintReadOnly)
	TEnumAsByte<EEffectApplicationPolicy> InfiniteEffectApplicationPolicy = E_DoNotApplyEffect;

	UPROPERTY(EditAnywhere, Category = "Applied Effect",BlueprintReadOnly,BlueprintReadOnly)
	TEnumAsByte<EEffectRemovalPolicy> InfiniteEffectRemovalPolicy = E_RemoveEffectOnEndOverlap;

	UPROPERTY(EditAnywhere, Category = "Applied Effect",BlueprintReadOnly,BlueprintReadOnly)
	bool bDestroyOnEffectRemoval = false;
};
