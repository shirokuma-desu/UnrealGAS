// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "MyPlayerController.generated.h"

class UDamageTextComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputConfig;
class IIInteraction;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class ACharacter;
/**
 * 
 */
UCLASS()
class AURA_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPlayerController();
	
	
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter);
	
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
	
	UPROPERTY(EditAnywhere, Category = "Input | Input Action")
	TObjectPtr<UInputAction> ShiftAction;
	
	
	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();
	void AutoRun();
	void OnShiftPressed(){ IsShiftDown = true; }
	void OnShiftReleased(){ IsShiftDown = false; }

	TScriptInterface<IIInteraction> LastActor;
	TScriptInterface<IIInteraction> ThisActor;
	
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);
	UAuraAbilitySystemComponent* GetASC();
	
	UPROPERTY(EditDefaultsOnly, Category = "Input ")
	TObjectPtr<UAuraInputConfig> InputConfig;
	
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	
	FHitResult CursorHitResult;
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool IsAutoRunning = false;
	bool IsTargeting = false;
	bool IsShiftDown = false;
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> SplineComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;
	
};
