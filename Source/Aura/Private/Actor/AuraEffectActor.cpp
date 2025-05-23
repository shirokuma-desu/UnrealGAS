// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"


// Sets default values
AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));
	
}


// Called when the game starts or when spawned
void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyGameplayEffectToTarget(AActor* ATarget, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
		//Get ASC of target Actor by using it static class, or you can cast manually like this
		/*UAbilitySystemInterface* ASCInterface = Cast<UAbilitySystemInterface>(ATarget);
		if (ASCInterface)
		{
			//Do something
		}*/
	
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ATarget);
		if (TargetASC == nullptr) return;
		check(GameplayEffectClass);
		//Create EffectContext from Target ASC
		//this context handle store information about the effect application, such as whom caused the effect
		//where it will happen, and other metadata.
		FGameplayEffectContextHandle GEContextHandle = TargetASC->MakeEffectContext();
		//Add SourceObj to ensure that effect know which actor or obj initiated it. Usefull for tracking, callback, or
		//who cause this effect. It just a container for contextual information.
		GEContextHandle.AddSourceObject(this);
		//Create GameplayEffectSpecHandle from the GameplayEffectClass. This is fully-defined instance of gameplay effect that is ready to be applied.
		const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass,ActorLevel,GEContextHandle);
		//You have created fully-defined instance of GEEffect, to apply it to the target ASC, you need to call ApplyGameplayEffectSpecToSelf
		//by using ASC
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	const bool bInfinite = EffectSpecHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
		if (bInfinite && InfiniteEffectRemovalPolicy == E_RemoveEffectOnEndOverlap)
		{
			ActiveGameplayEffects.Add(ActiveGameplayEffectHandle,TargetASC);
		}
		
}


void AAuraEffectActor::OnBeginOverlap(AActor* TargetActor)
{
	if (InstanceEffectApplicationPolicy == E_ApplyEffectOnBeginOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, InstanceGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == E_ApplyEffectOnBeginOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == E_ApplyEffectOnBeginOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstanceEffectApplicationPolicy == E_ApplyEffectOnEndOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, InstanceGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy == E_ApplyEffectOnEndOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfiniteEffectApplicationPolicy == E_ApplyEffectOnEndOverlap)
	{
		ApplyGameplayEffectToTarget(TargetActor, InfiniteGameplayEffectClass);
	}
	
	if (InfiniteEffectRemovalPolicy == E_RemoveEffectOnEndOverlap)
	{
		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!IsValid(TargetASC)) return;
		for (auto Effect : ActiveGameplayEffects)
		{
			if (TargetASC == Effect.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(Effect.Key,1);
				HandlesToRemove.Add(Effect.Key);
			}
		}
		for (FActiveGameplayEffectHandle& EffectHandle : HandlesToRemove)
		{
			ActiveGameplayEffects.FindAndRemoveChecked(EffectHandle);
		}
	}
}

void AAuraEffectActor::ApplyAllGameplayEffectToTarget(AActor* ATarget,
	TArray<TSubclassOf<UGameplayEffect>> GameplayEffects)
{
	for (auto Element : GameplayEffects)
	{
		ApplyGameplayEffectToTarget(ATarget, Element);
	}
}



