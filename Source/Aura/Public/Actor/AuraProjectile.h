// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraEffectActor.h"
#include "NiagaraSystem.h"
#include "AuraProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS()
class AURA_API AAuraProjectile : public AAuraEffectActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();
	
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 Otherbodyindex, bool bfromsweep, const FHitResult& SweepResult);
	
	
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactVFX;
	
	UPROPERTY(EditAnywhere)	
	TObjectPtr<USoundBase> ImpactSFX;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> HossingSFX;
	
	UPROPERTY()
	TObjectPtr<UAudioComponent> HossingSFXComponent;
	
	UPROPERTY(EditDefaultsOnly)
	float LifeSpanTime = 15.f;
	bool bHit;
};
