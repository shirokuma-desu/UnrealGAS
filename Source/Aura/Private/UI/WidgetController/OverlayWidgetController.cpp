// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"

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

