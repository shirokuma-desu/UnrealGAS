// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemBPLibrary.h"

#include "AbilitySystemComponent.h"
#include "AuraAbilityType.h"
#include "Game/MyGameMode.h"
#include "Interfaces/CombatInterface.h"
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

void UAuraAbilitySystemBPLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr) return;
	
	for (auto AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetCharacterClassDefaultInfo(CharacterClass);
	for (TSubclassOf AbilityClass : DefaultInfo.StartupAbilities)
	{
		ICombatInterface* CombatInterface =  Cast<ICombatInterface>(ASC->GetAvatarActor());
		if (CombatInterface)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(AbilitySpec);
		}
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

void UAuraAbilitySystemBPLibrary::GetLivePlayerWithinRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereQueryParams;
	SphereQueryParams.AddIgnoredActors(ActorToIgnore);
	TArray<FOverlapResult> OverlapResults;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(OverlapResults,SphereOrigin,FQuat::Identity,FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects),
			FCollisionShape::MakeSphere(Radius), SphereQueryParams);
		for (FOverlapResult& OverlapResult : OverlapResults)
		{
			if (OverlapResult.GetActor()->Implements<UCombatInterface>() &&  !ICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatarActor(OverlapResult.GetActor()));
			}
		}
	}
	
	
}

bool UAuraAbilitySystemBPLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool FirstIsPlayer = FirstActor->ActorHasTag(FName("Player"));
	const bool FirstIsEnemy = FirstActor->ActorHasTag(FName("Enemy"));
	const bool SecondIsPlayer = SecondActor->ActorHasTag(FName("Player"));
	const bool SecondIsEnemy = SecondActor->ActorHasTag(FName("Enemy"));
	
	const bool BothArePlayer = FirstIsPlayer && SecondIsPlayer;
	const bool BothAreEnemy = FirstIsEnemy && SecondIsEnemy;
	const bool Friend = BothAreEnemy || BothArePlayer;
	
	return !Friend;
}

