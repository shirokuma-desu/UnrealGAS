// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyOBJ = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility, "TargetDataUnderMouse");
	
	return  MyOBJ;
}

void UTargetDataUnderMouse::Activate()
{
	auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility,false,HitResult);
	ValidData.Broadcast(HitResult.Location);
	
}
