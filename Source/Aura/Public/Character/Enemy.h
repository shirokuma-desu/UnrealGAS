// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacter.h"
#include "Interfaces/IInteraction.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AEnemy : public AAuraCharacter, public IIInteraction
{
	GENERATED_BODY()
public:
	virtual void HightLightActor() override;
	virtual void UnHighLightActor() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bIsHighLight = false;
};
