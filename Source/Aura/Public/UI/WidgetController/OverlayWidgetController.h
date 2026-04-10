// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "OverlayWidgetController.generated.h"

struct FAuraAbilityInfo;
class UAuraAbilitySystemComponent;
class UAuraUserWidget;
struct FOnAttributeChangeData;

USTRUCT(BlueprintType)
//struct for datatable
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText MessageText = FText();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};


//Declare delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeDelegate,float ,NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatsChangeDelegate,int32 ,NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributesChangeDelegate,float,NewValue, float, OldValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowDelegate, FUIWidgetRow, Row);

	
/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS| Attributes")
	FOnAttributeChangeDelegate OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS| Attributes")
	FOnAttributeChangeDelegate OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS| Attributes")
	FOnAttributeChangeDelegate OnManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS| Attributes")
	FOnAttributeChangeDelegate OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS| Attributes")
	FMessageWidgetRowDelegate MessageWidgetRowHandler;
	
	
	
	
	UPROPERTY(BlueprintAssignable, Category = "GAS| XP")
    FOnAttributeChangeDelegate OnXPPercentChangeDelegate;
	
	UPROPERTY(BlueprintAssignable, Category = "GAS| XP")
	FOnPlayerStatsChangeDelegate OnPlayerLevelChangeDelegate;


	
protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Widget Data")
	TObjectPtr<UDataTable> MessageDataTable;

	template<typename T>
	T* GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag);
	
	void OnXPChanged(int32 NewXP);
	
	
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));
}
