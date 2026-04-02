// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Player/MyPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	check(AttributeInformation)
	for (auto& Pair : AS->TagToAttribute)
	{
		BoardCastAttributeInfo(Pair.Key,Pair.Value());
	}
	AMyPlayerState* MyPlayerState = CastChecked<AMyPlayerState>(PlayerState);
	AttributePointsChangeDelegate.Broadcast(MyPlayerState->GetAttributePoints());
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{	
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	for (auto& Pair : AS->TagToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair](const FOnAttributeChangeData& Data)
		{
				BoardCastAttributeInfo(Pair.Key,Pair.Value());
		});
	}
	
	AMyPlayerState* MyPlayerState = CastChecked<AMyPlayerState>(PlayerState);
	MyPlayerState->OnAttributePointsChangeDelegate.AddLambda([this](int32 Points)
	{
			AttributePointsChangeDelegate.Broadcast(Points);
	});
}

void UAttributeMenuWidgetController::BoardCastAttributeInfo(const FGameplayTag& AttributeTag,
	const FGameplayAttribute& Attribute) const
{
	FAuraAttributeInfo Info = AttributeInformation->FindAttributeInfoForTag(AttributeTag);	
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}

