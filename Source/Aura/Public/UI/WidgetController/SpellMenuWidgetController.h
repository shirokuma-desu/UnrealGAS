// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayTag.h"
#include "GameplayTagContainer.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, bool, bSpellPointButtonEnable, bool,bEquipButtonEnable, FString, DescriptionString, FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature,const FGameplayTag& ,AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReAssignedSignature,const FGameplayTag& ,AbilityTag);
struct  FSelectedAbility
{
	FGameplayTag Ability = FGameplayTag();
	FGameplayTag Status = FGameplayTag();
	
};

UCLASS(Blueprintable,BlueprintType)
class AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
public:
	 virtual void BroadcastInitialValues() override;
	 virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatsChangeSignature SpellPointChanged;
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature WaitForEquipDelegate;
	UPROPERTY(BlueprintAssignable)
	FWaitForEquipSelectionSignature StopWaitForEquipDelegate;
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeReAssignedSignature SpellGlobeReAssignedDelegate;
	
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);
	
	
	UFUNCTION(BlueprintCallable)
	void SpendPointButtonPress();
	
	
	UPROPERTY(BlueprintAssignable)
	FSpellGlobeSelectedSignature SpellGlobeSelectedDelegate;
	
	UFUNCTION(BlueprintCallable)
	void GlobeDeselect();
	
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();
	
	UFUNCTION(BlueprintCallable)
	void SpellGlobePressed(const FGameplayTag& SlotTag,const FGameplayTag& AbilityType );
	
	void OnAbilityEquipped(const FGameplayTag& AbilityTag,const FGameplayTag& Status , const FGameplayTag& Slot, const FGameplayTag& PrevSlot );
	
private:
	static void ShouldEnableButton(const FGameplayTag& AbilityStatus, int32 SpellPoints, 
		bool& bshouldEnableSpellButtons, bool& bShouldEnableEquipButton);
	
	FSelectedAbility SelectedAbility = {FAuraGameplayTag::Get().Ability_None, FAuraGameplayTag::Get().Ability_Status_Lock};
	
	int32 CurrenSpellPoints = 0;
	
	bool bWaitingForEquipSelection = false;
	
	FGameplayTag SelectedSlot;
};
