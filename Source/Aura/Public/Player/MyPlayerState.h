// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

class ULevelUpInfo;
class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatsChange, int32 StatValue)

UCLASS()
class AURA_API AMyPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	AMyPlayerState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	
	void AddToXp(int32 InXP);
	void SetXP(int32 InXP);
	void AddToLevel(int32 InLevel);
	void SetLevel(int32 InLevel);
	void AddToAttributePoints(int32 InAttributePoints);
	void AddToSpellPoints(int32 InSpellPoints);
	
	//delegate
	FOnPlayerStatsChange OnXPChangeDelegate;
	FOnPlayerStatsChange OnLevelChangeDelegate;
	FOnPlayerStatsChange OnSpellPointsChangeDelegate;
	FOnPlayerStatsChange OnAttributePointsChangeDelegate;
	
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfoDataAsset;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	
	
private:
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing= OnRep_Level)
	int32 Level = 1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing= OnRep_XP)
	int32 XP = 1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing= OnRep_AttributePoints)
	int32 AttributePoints = 0;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing= OnRep_SpellPoints)
	int32 SpellPoints = 0;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	
	UFUNCTION()
	void OnRep_XP(int32 OldXP);
	
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);
	
	UFUNCTION()
	void OnRep_SpellPoints(int32 OldSpellPoints);
};
