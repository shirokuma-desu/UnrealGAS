// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Class Default")
	TSubclassOf<class UGameplayEffect> PrimaryAttributeGameplayEffect;
	
};

/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Default")
	TMap<ECharacterClass,FCharacterClassDefaultInfo> CharacterClassInformationMap;
	
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TSubclassOf< UGameplayEffect> SecondaryAttributeGameplayEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TSubclassOf< UGameplayEffect> VitalAttributeGameplayEffect;
	
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "Common Class Default| Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficient;
	
	FCharacterClassDefaultInfo GetCharacterClassDefaultInfo(ECharacterClass CharacterClass);
};
