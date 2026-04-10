// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/MyPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	
	OnHealthChanged.Broadcast(GetAuraAttributeSet()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetAuraAttributeSet()->GetMaxHealth());
	OnManaChanged.Broadcast(GetAuraAttributeSet()->GetMana());
	OnMaxManaChanged.Broadcast(GetAuraAttributeSet()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetMyPlayerState()->OnXPChangeDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChanged);
	GetMyPlayerState()->OnLevelChangeDelegate.AddLambda([this](int32 NewLevel)
	{
		OnPlayerLevelChangeDelegate.Broadcast(NewLevel);
	});
	
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetAuraAttributeSet()->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
		);
	
	if (GetAuraAbilitySystemComponent())
	{
		if (GetAuraAbilitySystemComponent()->bStartupAbilityGiven)
		{
			BoardCastAbilityInfo();
		}
		else
		{
			GetAuraAbilitySystemComponent()->OnAbilityGivenHandler.AddUObject(this,&UOverlayWidgetController::BoardCastAbilityInfo);
		}
		
		GetAuraAbilitySystemComponent()->OnEffectAssetTagsHandler.AddLambda(
	[this](const FGameplayTagContainer& AssetTags)
	{
		/*
		 *	So to check which tag in array tag is the tag we need
		 *  using FGameplayTag::RequestGameplayTag to get the tag we want
		 *  then using .MatchesTag(param Tag) to check
		 */
		for (const auto& Tag : AssetTags)
		{
			FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
			if (Tag.MatchesTag(MessageTag))
			{
			const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageDataTable,Tag);
			MessageWidgetRowHandler.Broadcast(*Row);
			}
			
		}
	}
	);
	}
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetMyPlayerState()->LevelUpInfoDataAsset;
	checkf(LevelUpInfo, TEXT("Unable To find LevelUpInfo. Please fill out Myplayerstate Blueprint"));
	
	const int32 Level  = LevelUpInfo->FindLevelUpInforXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformations.Num();
	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformations[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformations[Level -1].LevelUpRequirement;
		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;
		
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);
		
		OnXPPercentChangeDelegate.Broadcast(XPBarPercent);
	}
}

