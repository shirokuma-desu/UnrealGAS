// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyOBJ = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility, "TargetDataUnderMouse");
	
	return  MyOBJ;
}

void UTargetDataUnderMouse::Activate()
{
	
	const bool bIsLocallyControlled = Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocallyControlled)
	{
		SendMouseCursorData();
	}
	else
	{
		FGameplayAbilitySpecHandle SpecHandle = GetAbilitySpecHandle();
		FPredictionKey ActivationPredictionKey = GetActivationPredictionKey();
		//To do: we are on server so listen for target data
		AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(SpecHandle, ActivationPredictionKey).
		AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicateCallback);
		const bool  bCalledDelegate = AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle,ActivationPredictionKey);
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
	
	
	
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	FScopedPredictionWindow ScopedPredictionWindow(AbilitySystemComponent.Get());
	
	FGameplayAbilityTargetData_SingleTargetHit* HitTargetData = new FGameplayAbilityTargetData_SingleTargetHit();
	auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility,false,HitResult);
	FGameplayAbilityTargetDataHandle TargetDataHandle;
	HitTargetData->HitResult = HitResult;
	TargetDataHandle.Add(HitTargetData);
	AbilitySystemComponent->ServerSetReplicatedTargetData(GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		TargetDataHandle,FGameplayTag(),AbilitySystemComponent->ScopedPredictionKey);
	
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(TargetDataHandle);
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicateCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	FGameplayTag ActivationTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(),GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
