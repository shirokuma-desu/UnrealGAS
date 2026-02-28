// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemBPLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityType.h"
#include "Game/MyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Player/MyPlayerState.h"
#include "UI/HUD/MyHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemBPLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AMyHUD* AuraHUD = Cast<AMyHUD>(PC->GetHUD()))
		{
			AMyPlayerState* PS = PC->GetPlayerState<AMyPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(ASC,AS,PC,PS);
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemBPLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AMyHUD* AuraHUD = Cast<AMyHUD>(PC->GetHUD()))
		{
			AMyPlayerState* PS = PC->GetPlayerState<AMyPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(ASC,AS,PC,PS);
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemBPLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, float Level,  UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();
	
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo) return;
	FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	
	FGameplayEffectContextHandle PrimaryAttributeEffectContextHandle = ASC->MakeEffectContext();
	PrimaryAttributeEffectContextHandle.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = 
		ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributeGameplayEffect,Level,PrimaryAttributeEffectContextHandle);
	
	FGameplayEffectContextHandle SecondaryAttributeEffectContextHandle = ASC->MakeEffectContext();
	SecondaryAttributeEffectContextHandle.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = 
		ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributeGameplayEffect,Level,SecondaryAttributeEffectContextHandle);
	
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());
	
	FGameplayEffectContextHandle VitalAttributeEffectContextHandle = ASC->MakeEffectContext();
	VitalAttributeEffectContextHandle.AddSourceObject(AvatarActor);
	
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = 
		ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributeGameplayEffect,Level,VitalAttributeEffectContextHandle);
	
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
	

}

void UAuraAbilitySystemBPLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo) return;
	
	for (auto AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		ASC->GiveAbility(AbilitySpec);
	}
}

UCharacterClassInfo* UAuraAbilitySystemBPLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	AMyGameMode* AuraGameMode = Cast<AMyGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (AuraGameMode == nullptr) return nullptr;
	return AuraGameMode->CharacterClassInfo;
}

bool UAuraAbilitySystemBPLibrary::IsBlockHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->IsBlockHit();
	}
	return false;
}

bool UAuraAbilitySystemBPLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystemBPLibrary::SetIsBlockHit(FGameplayEffectContextHandle& EffectContextHandle, bool IsBlockHit)
{
	if ( FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->SetBlockHit(IsBlockHit);
	}
}

void UAuraAbilitySystemBPLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool IsCriticalHit)
{
	if ( FAuraGameplayEffectContext* AuraGameplayEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraGameplayEffectContext->SetCriticalHit(IsCriticalHit);
	}
}

