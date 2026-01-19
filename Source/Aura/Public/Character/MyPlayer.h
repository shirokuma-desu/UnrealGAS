// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacter.h"
#include "MyPlayer.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AMyPlayer : public AAuraCharacter
{
	GENERATED_BODY()
public:
	AMyPlayer();

	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	//*Combat Interface
	virtual int32 GetPlayerLevel() override;
	
private:
	virtual void InitAbilityActorInfo() override;
};
