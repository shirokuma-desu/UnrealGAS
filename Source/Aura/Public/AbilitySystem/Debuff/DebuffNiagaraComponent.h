// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "DebuffNiagaraComponent.generated.h"



UCLASS()
class AURA_API UDebuffNiagaraComponent : public UNiagaraComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDebuffNiagaraComponent();
	
	UPROPERTY(VisibleAnywhere)
	FGameplayTag DebuffTag;
protected:
	virtual void BeginPlay() override;
	void DebuffTagChanged(const FGameplayTag CallBackTag,int32 NewCount);
	
	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};
