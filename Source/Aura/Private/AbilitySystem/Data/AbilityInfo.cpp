// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

#include "Aura/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(const FGameplayTag& AbilityTag, bool bLogNotFound) const
{
	for(const auto AuraAbilityInfo : AbilitiesInformation)
	{
		if (AuraAbilityInfo.AbilityTag == AbilityTag)
		{
			return AuraAbilityInfo;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("Cant Find Info For Ability Tag [%s] on AbilityInfo [%s] "), *AbilityTag.ToString(),*GetNameSafe(this));
	}
	
	return FAuraAbilityInfo();
}
