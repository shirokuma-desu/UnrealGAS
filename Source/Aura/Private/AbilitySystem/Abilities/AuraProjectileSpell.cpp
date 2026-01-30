// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interfaces/CombatInterface.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	const bool IsServer = HasAuthority(&ActivationInfo);
	if (!IsServer) return;
	
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	
	if (!CombatInterface) return;
	auto CombatSocketLocation = CombatInterface->GetCombatSocketLocation();
	FTransform SpawnTransform;
	//To do: set projectile rotation
	SpawnTransform.SetLocation(CombatSocketLocation);
	AAuraProjectile* Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,SpawnTransform, GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()), ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	//To do: Give project tile a gameplay effect spec for causing damage.
	
	Projectile->FinishSpawning(SpawnTransform);
}
