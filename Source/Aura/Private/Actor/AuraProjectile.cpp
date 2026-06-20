// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystemBPLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	SphereCollision = CreateDefaultSubobject<USphereComponent>("SphereCollision");
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	
	bReplicates = true;
	
	SetRootComponent(SphereCollision);
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereCollision->SetCollisionObjectType(ECC_Projectile);
	
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnSphereOverlap);
	HissSFXComponent = UGameplayStatics::SpawnSoundAttached(HissSFX,GetRootComponent());
	SetLifeSpan(LifeSpanTime);
}

void AAuraProjectile::Destroyed()
{
	if (HissSFXComponent)
	{
		HissSFXComponent->Stop();
		HissSFXComponent->DestroyComponent();
	}
	
	if (!bHit && !HasAuthority())
	{
		OnHit();
	}
	Super::Destroyed();
}

void AAuraProjectile::OnHit()
{
	UGameplayStatics::PlaySoundAtLocation(this,ImpactSFX,GetActorLocation(),FRotator::ZeroRotator);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,ImpactVFX,GetActorLocation());
	if (HissSFXComponent)
	{
		HissSFXComponent->Stop();
		HissSFXComponent->DestroyComponent();
	}
	bHit = true;
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 Otherbodyindex, bool bfromsweep, const FHitResult& SweepResult)
{
	
	 AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	
	if (SourceAvatarActor ==  OtherActor) return;
	if (!UAuraAbilitySystemBPLibrary::IsNotFriend(SourceAvatarActor,OtherActor)) return;
	
	if (!bHit) OnHit();
	
	if (HasAuthority())
	{
		if(auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const FVector DeathImpulse = GetActorForwardVector() *DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;
			const bool bKnockback = FMath::RandRange(1, 100) < DamageEffectParams.KnockbackChance;
			if (bKnockback)
			{
				FRotator Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				const FVector KnockbackDirection = Rotation.Vector();
				const FVector KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
				DamageEffectParams.KnockbackForce = KnockbackForce;
			}
			
			DamageEffectParams.TargetAbilitySystemComponent = TargetASC;
			UAuraAbilitySystemBPLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}


