// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemBPLibrary.generated.h"

struct FDamageEffectParams;
class UAbilityInfo;
class AMyHUD;
struct FWidgetControllerParams;
class USpellMenuWidgetController;
struct FGameplayEffectContextHandle;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	public:
	
	UFUNCTION(BlueprintPure, Category= "AuraAbiltySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerParam(const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetControllerParams, AMyHUD*& OutMyHUD);
	
	UFUNCTION(BlueprintPure, Category= "AuraAbiltySystemLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category= "AuraAbiltySystemLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController (const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category= "AuraAbiltySystemLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController (const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category= "AuraAbiltySystemLibrary|CharacterClassDefault")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject,ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC);
	
	UFUNCTION(BlueprintCallable, Category= "AuraAbiltySystemLibrary|CharacterClassDefault")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);
	
	UFUNCTION(BlueprintCallable, Category= "AuraAbiltySystemLibrary|CharacterClassDefault")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category= "AuraAbiltySystemLibrary|CharacterClassDefault")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category= "AuraAbiltySystemLibrary| GameplayEffect")
	static bool IsBlockHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category= "AuraAbiltySystemLibrary| GameplayEffect")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	
	UFUNCTION(BlueprintPure, Category= "AuraAbiltySystemLibrary| GameplayEffect")
	static void SetIsBlockHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool IsBlockHit);
	
	UFUNCTION(BlueprintPure, Category= "AuraAbiltySystemLibrary| GameplayEffect")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool IsCriticalHit);
	
	UFUNCTION(BlueprintCallable, Category= " AuraAbiltySystemLibrary |Gameplay Mechanic")
	static void GetLivePlayerWithinRadius (const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorToIgnore, float Radius, const FVector& SphereLocation);
	
	UFUNCTION(BlueprintPure, Category= "AuraAbiltySystemLibrary|Gameplay Mechanic")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor); 
	 
	UFUNCTION(BlueprintCallable, Category= " AuraAbiltySystemLibrary |Damage Effect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
		
	
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject,ECharacterClass CharacterClass, int32 CharacterLevel);
};
