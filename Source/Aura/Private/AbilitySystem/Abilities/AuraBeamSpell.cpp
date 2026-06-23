// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraBeamSpell.h"

#include "AbilitySystem/AuraAbilitySystemBPLibrary.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"

void UAuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit ==true)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UAuraBeamSpell::StoreOwnerVariables()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}
}

void UAuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	check(OwnerCharacter)
	if (OwnerCharacter->Implements<UCombatInterface>())
	{
		if (USkeletalMeshComponent* Weapon = ICombatInterface::Execute_GetWeapon(OwnerCharacter))
		{
			const FVector SocketLocation = Weapon->GetSocketLocation(FName("TipSocket"));
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(OwnerCharacter);
			FHitResult HitResult;
			UKismetSystemLibrary::SphereTraceSingle(OwnerCharacter,SocketLocation,BeamTargetLocation,10.f,
				TraceTypeQuery1, false,ActorsToIgnore, 
				EDrawDebugTrace::None,HitResult, true);
			
			if (HitResult.bBlockingHit)
			{
				MouseHitLocation = HitResult.ImpactPoint;
				MouseHitActor = HitResult.GetActor();
			}
			
		}
	}
	if (ICombatInterface* CombatInterface =  Cast<ICombatInterface>(MouseHitActor))
	{
		if (!CombatInterface->GetOnDeadDelegate().IsAlreadyBound(this,&UAuraBeamSpell::PrimaryTargetDied))
		{
		CombatInterface->GetOnDeadDelegate().AddDynamic(this, &UAuraBeamSpell::PrimaryTargetDied);
		}
	}
	
}

void UAuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTarget)
{
	TArray<AActor*> OverlappingActors;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());
	ActorsToIgnore.Add(MouseHitActor);
	
	UAuraAbilitySystemBPLibrary::GetLivePlayerWithinRadius(GetAvatarActorFromActorInfo(),
		OverlappingActors,ActorsToIgnore,850.f,MouseHitActor->GetActorLocation());
	
	//int32 NumAdditionalTarget = 5;
	int32 NumAdditionalTarget = FMath::Min(GetAbilityLevel(),MaxNumShockTargets);
	
	UAuraAbilitySystemBPLibrary::GetClosestTargets(NumAdditionalTarget,OverlappingActors,OutAdditionalTarget
		,MouseHitActor->GetActorLocation());
	
	for (AActor* Target : OutAdditionalTarget)
	{
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(Target))
		{
			if (!CombatInterface->GetOnDeadDelegate().IsAlreadyBound(this,&UAuraBeamSpell::AdditionalTargetDied))
			{
				CombatInterface->GetOnDeadDelegate().AddDynamic(this, &UAuraBeamSpell::AdditionalTargetDied);
			}
		}
	}
	
}
