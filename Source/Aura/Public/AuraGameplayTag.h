// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Aura GameplayTag
 * Singleton containing native gameplayTag
 */
struct FAuraGameplayTag
{
public:
	static const FAuraGameplayTag& Get(){return GameplayTag; };
	static void InitializeNativeGameplayTag();
	
	FGameplayTag Attributes_Secondary_Armor;
protected:
	
private:
	static FAuraGameplayTag GameplayTag;
};


