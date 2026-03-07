// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interfaces/IInteraction.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "Enemy.generated.h"

class UBehaviorTree;
class AAuraAIController;
class UWidgetComponent;
//Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHoverEnemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotHoverEnemy);

/**
 * 
 */
UCLASS()
class AURA_API AEnemy : public AAuraCharacterBase, public IIInteraction
{
	GENERATED_BODY()
public:
	
	AEnemy();
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeDelegate OnHealthChange;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangeDelegate OnMaxHealthChange;
	
	
	UPROPERTY(BlueprintAssignable, Category = "Events|Highlight")
	FOnHoverEnemy OnHoverHappened;
	UPROPERTY(BlueprintAssignable, Category = "Events|Highlight")
	FOnNotHoverEnemy OnNotHoverEnemyHappened;
	//* Interaction interface
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	
	//* Combat Interface
	virtual int32 GetPlayerLevel() override;
	virtual  void Die() override;
	
	virtual void PossessedBy(AController* NewController) override;
	
	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
	
	UPROPERTY(BlueprintReadOnly, Category= "Combat")
	bool bHitReacting = false;
	
	UPROPERTY(BlueprintReadOnly, Category= "Combat")
	float BaseWalkSpeed = 250.f;
	
	UPROPERTY(EditAnywhere, Category= "Combat")
	float LifeSpan = 5.f;
	
	UPROPERTY(BlueprintReadWrite, Category= "Combat")
	TObjectPtr<AActor> CombatTarget;
	
protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsHighLight = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Character Class Default")
	int32 Level = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category= "Character Class Default")
	ECharacterClass ECharacterClass = ECharacterClass::Warrior;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Character Class Default")
	TObjectPtr<UWidgetComponent> HealthBar;
	
	UPROPERTY(EditDefaultsOnly, Category= "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
	
	
 	
};
