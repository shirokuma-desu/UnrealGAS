// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacter.h"
#include "Interfaces/IInteraction.h"
#include "Enemy.generated.h"

//Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHoverEnemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotHoverEnemy);

/**
 * 
 */
UCLASS()
class AURA_API AEnemy : public AAuraCharacter, public IIInteraction
{
	GENERATED_BODY()
public:
	
	AEnemy();
	
	UPROPERTY(BlueprintAssignable, Category = "Events|Highlight")
	FOnHoverEnemy OnHoverHappened;
	UPROPERTY(BlueprintAssignable, Category = "Events|Highlight")
	FOnNotHoverEnemy OnNotHoverEnemyHappened;
	//* Interaction interface
	virtual void HightLightActor() override;
	virtual void UnHighLightActor() override;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsHighLight = false;
};
