// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraSummonAbility.h"

#include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	
	const float DeltaSpread = SpawnSpread / NumberMinionsToSpawn;
	
	const FVector LeftOfSpread = Forward.RotateAngleAxis( -SpawnSpread*0.5f, FVector::UpVector);
	TArray<FVector> SpawnLocations;
	for (int32 i= 0; i < NumberMinionsToSpawn; i++)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread*i, FVector::UpVector);
		FVector ChosenSpawnLocation =  Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
		
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit, ChosenSpawnLocation + FVector(0.f, 0.f, 400.f), ChosenSpawnLocation - FVector(0.f, 0.f, 400.f), ECC_Visibility);
		if (Hit.bBlockingHit)
		{
			ChosenSpawnLocation = Hit.ImpactPoint;
		}
		
		SpawnLocations.Add(ChosenSpawnLocation);
	}
	
	return SpawnLocations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionsClass()
{
	if (!MinionsClass.IsEmpty())
	{
		int32 RandomIndex = FMath::RandRange(0, MinionsClass.Num() - 1);
		return MinionsClass[RandomIndex];
	}
	return nullptr;
}
