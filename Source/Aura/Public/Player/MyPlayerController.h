// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

class IIInteraction;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class AURA_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPlayerController();
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

private:
	//input mapping context
	UPROPERTY(EditAnywhere, Category="Input | Input Mapping Context")
	TObjectPtr<UInputMappingContext> PlayerInputMappingContext;
	
	//input action
	UPROPERTY(EditAnywhere, Category = "Input | Input Action")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

	TScriptInterface<IIInteraction> LastActor;
	TScriptInterface<IIInteraction> ThisActor;
	
};
