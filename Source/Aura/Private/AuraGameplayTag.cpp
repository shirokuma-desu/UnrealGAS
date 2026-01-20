// Fill out your copyright notice in the Description page of Project Settings.


#include "AuraGameplayTag.h"

#include "GameplayTagsManager.h"


FAuraGameplayTag FAuraGameplayTag::GameplayTag;

void FAuraGameplayTag::InitializeNativeGameplayTag()
{
	GameplayTag.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces Damage Taken, improves block chance"));
}

