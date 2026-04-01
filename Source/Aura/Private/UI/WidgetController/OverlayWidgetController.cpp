// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Player/MyPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	AMyPlayerState* MyPlayerState = CastChecked<AMyPlayerState>(PlayerState);
	MyPlayerState->OnXPChangeDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChanged);
	MyPlayerState->OnLevelChangeDelegate.AddLambda([this](int32 NewLevel)
	{
		OnPlayerLevelChangeDelegate.Broadcast(NewLevel);
	});
	
	
	
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		}
		);
	
	if (auto AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (AuraASC->bStartupAbilityGiven)
		{
			OnInitializeStartupAbility(AuraASC);
		}
		else
		{
			AuraASC->OnAbilityGivenHandler.AddUObject(this,&UOverlayWidgetController::OnInitializeStartupAbility);
		}
		
		AuraASC->OnEffectAssetTagsHandler.AddLambda(
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

void UOverlayWidgetController::OnInitializeStartupAbility(UAuraAbilitySystemComponent* AuraASC)
{
	//Todo get infomation about all given abilities, look up their ability info, and boardcast it to widget
	if (!AuraASC->bStartupAbilityGiven) return;
	
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this,AuraASC](const FGameplayAbilitySpec& AbilitySpec	)
	{
		//Todo Need a way to figure out ability tag for given ability spec.
		
		auto Info = AbilityInfo->FindAbilityInfoByTag(AuraASC->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = AuraASC->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
		});
	
	AuraASC->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const AMyPlayerState* MyPlayerState = CastChecked<AMyPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = MyPlayerState->LevelUpInfoDataAsset;
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

