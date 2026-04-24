// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"

#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "AbilitySystem/Data/DA_AttributeInfo.h"
#include "Player/MyPlayerState.h"

void USpellMenuWidgetController::BroadcastInitialValues()
{
	BoardCastAbilityInfo();
	SpellPointChanged.Broadcast(GetMyPlayerState()->GetSpellPoints());
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetAuraAbilitySystemComponent()->OnAbilityStatusChangedHandler.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag,int32 NewLevel)
	{
		if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
		{
			SelectedAbility.Status = StatusTag;
			bool bEnableSpendPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButton(StatusTag,CurrenSpellPoints, bEnableSpendPoints, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(AbilityTag,Description,NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints,bEnableEquip,Description,NextLevelDescription);
		}
		
		if (AbilityInfo)
		{
			FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
			Info.StatusTag = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		}
		
	});
	GetAuraAbilitySystemComponent()->AbilityEquipped.AddUObject(this, &USpellMenuWidgetController::OnAbilityEquipped);
	
	GetMyPlayerState()->OnSpellPointsChangeDelegate.AddLambda([this](int32 SpellPoints)
	{
		SpellPointChanged.Broadcast(SpellPoints);
		CurrenSpellPoints = SpellPoints;
		
		
		bool bEnableSpendPoints = false;
		bool bEnableEquip = false;
		ShouldEnableButton(SelectedAbility.Status,CurrenSpellPoints, bEnableSpendPoints, bEnableEquip);
		FString Description;
		FString NextLevelDescription;
		GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(SelectedAbility.Ability,Description,NextLevelDescription);
		SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints,bEnableEquip,Description,NextLevelDescription);
		
	});
	
	
	
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
		StopWaitForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	
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
	
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;
	
	bool bEnableSpendPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButton(AbilityStatus,SpellPoints, bEnableSpendPoints, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetAuraAbilitySystemComponent()->GetDescriptionByAbilityTag(AbilityTag,Description,NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpendPoints,bEnableEquip,Description,NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPress()
{
	if (GetAuraAbilitySystemComponent())
	{
		GetAuraAbilitySystemComponent()->ROS_SpendSpellPoints(SelectedAbility.Ability);
	}
}


void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipSelection)
	{
		FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
		StopWaitForEquipDelegate.Broadcast(SelectedAbilityType);
		bWaitingForEquipSelection = false;
	}
	SelectedAbility.Ability = FAuraGameplayTag::Get().Ability_None;
	SelectedAbility.Status = FAuraGameplayTag::Get().Ability_Status_Lock;
	SpellGlobeSelectedDelegate.Broadcast(false,false,FString(),FString());
	
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag AbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
	
	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;
	
	const FGameplayTag SelectedStatus = GetAuraAbilitySystemComponent()->GetStatusFromAbilityTag(SelectedAbility.Ability);
	if (SelectedStatus.MatchesTagExact(FAuraGameplayTag::Get().Ability_Status_Equipped))
	{
		SelectedSlot = GetAuraAbilitySystemComponent()->GetInputTagFromAbilityTag(SelectedAbility.Ability);
	}
}

void USpellMenuWidgetController::SpellGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType)
{
	if (!bWaitingForEquipSelection) return;
	//check selected ability against the slot's ability. (don't equip an offensive spell in a passive slot and vice versa)
	const FGameplayTag SelectedAbilityType = AbilityInfo->FindAbilityInfoByTag(SelectedAbility.Ability).AbilityType;
	if (!SelectedAbilityType.MatchesTagExact(AbilityType)) return;	
	
	GetAuraAbilitySystemComponent()->ROS_EquipAbility(SelectedAbility.Ability, SlotTag);
	
}

void USpellMenuWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status,
	const FGameplayTag& Slot, const FGameplayTag& PrevSlot)
{
	bWaitingForEquipSelection = false;
	const FAuraGameplayTag& GameplayTag = FAuraGameplayTag::Get();
	
	FAuraAbilityInfo LastSlotInfo; 
	LastSlotInfo.StatusTag = GameplayTag.Ability_Status_Unlocked;
	LastSlotInfo.InputTag = PrevSlot;
	LastSlotInfo.AbilityTag = GameplayTag.Ability_None;
	//Broadcast empty info if Prev slot is a valid slot. Only if equipping an already-equipped spell
	AbilityInfoDelegate.Broadcast(LastSlotInfo);
	FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
	
	StopWaitForEquipDelegate.Broadcast(AbilityInfo->FindAbilityInfoByTag(AbilityTag).AbilityType);
	SpellGlobeReAssignedDelegate.Broadcast(AbilityTag);
	GlobeDeselect();
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
