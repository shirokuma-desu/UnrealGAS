// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Player/MyPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BoardCastAbilityInfo();
	SpellPointChanged.Broadcast(GetMyPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraAbilitySystemComponent()->OnAbilityStatusChangedHandler.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
	});
	
	GetMyPlayerState()->OnSpellPointsChangeDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointChanged.Broadcast(SpellPoints);
	});
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const FAuraGameplayTag GameplayTags = FAuraGameplayTag::Get();
	const int32 SpellPoints = GetMyPlayerState()->GetSpellPoints();
	FGameplayTag AbilityStatus;
	
	const bool bTagValid = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(FAuraGameplayTag::Get().Ability_None);
	const bool bSpecValid = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag) != nullptr;
	const FGameplayAbilitySpec* AbilitySpec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);
	if (!bTagValid || bTagNone || !bSpecValid)
	{
		AbilityStatus = GameplayTags.Ability_Status_Lock;
	}
	else
	{
		AbilityStatus = GetAuraAbilitySystemComponent()->GetStatusFromSpec(*AbilitySpec);
	}
	
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButton(AbilityStatus,SpellPoints, bEnableSpendPoints, bEnableEquip);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints,bEnableEquip);
}

void USpellMenuWidgetController::ShouldEnableButton(const FGameplayTag& AbilityStatus, int32 SpellPoints,
	bool& bshouldEnableSpellButtons, bool& bShouldEnableEquipButton)
{
	const FAuraGameplayTag GameplayTags = FAuraGameplayTag::Get();
	bshouldEnableSpellButtons = false;
	bShouldEnableEquipButton = false;
	
	if (AbilityStatus.MatchesTagExact(GameplayTags.Ability_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints>0)
		{
			bshouldEnableSpellButtons = true;
		}	
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Ability_Status_Eligible))
	{
		if (SpellPoints>0)
		{
			bshouldEnableSpellButtons = true;
		}	
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTags.Ability_Status_Unlocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints>0)
		{
			bshouldEnableSpellButtons = true;
		}	
	}
	
}
