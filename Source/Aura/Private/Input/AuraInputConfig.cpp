// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

const UInputAction* UAuraInputConfig::FindAbilityInputAction(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const auto& Element : AbilityInputActions)
	{
		if (Element.InputAction && Element.InputTag == InputTag)
		{
			return Element.InputAction;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error,TEXT("Cant Find Ability Input Action  or input tag [%s], on inputconfig"),*InputTag.ToString(),*GetNameSafe(this));
	}
	
	return nullptr;	
}
