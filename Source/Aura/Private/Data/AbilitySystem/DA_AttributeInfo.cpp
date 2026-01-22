// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/AbilitySystem/DA_AttributeInfo.h"

FAuraAttributeInfo UDA_AttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag,
	bool bLogNotFround) const
{
	for ( auto Info : AttributeInfos )
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	
	if (bLogNotFround == true)
	{
		UE_LOG(LogTemp, Error, TEXT("Can find infor attributeTag : %s on AttributeInfo: %s"),
			*AttributeTag.ToString(),*GetNameSafe(this));
	}
	return FAuraAttributeInfo();
}
