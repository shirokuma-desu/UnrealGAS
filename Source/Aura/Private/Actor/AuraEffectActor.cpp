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

void AAuraEffectActor::ApplyGameplayEffectToTarget(AActor* ATarget, TSubclassOf<UGameplayEffect> EGameplayEffectClass)
{
		//Get ASC of target Actor by using it static class, or you can cast manually like this
		/*UAbilitySystemInterface* ASCInterface = Cast<UAbilitySystemInterface>(ATarget);
		if (ASCInterface)
		{
			//Do something
		}*/
	
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(ATarget);
		if (TargetASC == nullptr) return;
		check(EGameplayEffectClass);
		//Create EffectContext from Target ASC
		FGameplayEffectContextHandle GEContextHandle = TargetASC->MakeEffectContext();
		//Create SourceObject which mean this target actor is object do that effect.
		GEContextHandle.AddSourceObject(this);
		//Create GESpec contain all necessary information 
		const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EGameplayEffectClass,1.f,GEContextHandle);
		//now apply that spec to a target actor
		TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}


