// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTag.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "Input/AuraInputComponent.h"
#include "Interfaces/IInteraction.h"

AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	checkCode(
		if (PlayerInputMappingContext == nullptr)
		{
			UE_LOG(LogUObjectGlobals,Fatal, TEXT("Begin Play, Player Input Mapping does not have ref"));
		}
		);

	UEnhancedInputLocalPlayerSubsystem* Subsytem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	//init IMC
	if (Subsytem)
	{
	Subsytem->AddMappingContext(PlayerInputMappingContext, 0);
	}
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	//config cursor
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AMyPlayerController::Move);
	AuraInputComponent->BindAbilityAction(InputConfig,this,&ThisClass::AbilityInputPressed,
		&ThisClass::AbilityInputReleased, &ThisClass::AbilityInputHeld);
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
}

void AMyPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector2D = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotator(0,Rotation.Yaw,0);

	const FVector ForwardDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

	if (APawn* ControllerPawn = GetPawn())
	{
		ControllerPawn->AddMovementInput(ForwardDirection,InputAxisVector2D.Y);
		ControllerPawn->AddMovementInput(RightDirection,InputAxisVector2D.X);
	}
}

void AMyPlayerController::CursorTrace()
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,HitResult);
	if (!HitResult.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();

	if (!LastActor && !ThisActor)
	{
		// A. Cả hai đều null -> Không làm gì
	}
	else if (!LastActor && ThisActor)
	{
		// B. Chỉ ThisActor có giá trị -> Highlight ThisActor
		ThisActor->HightLightActor();
	}
	else if (LastActor && !ThisActor)
	{
		// C. Chỉ LastActor có giá trị -> UnHighlight LastActor
		LastActor->UnHighLightActor();
	}
	else if (LastActor != ThisActor)
	{
		// D. Cả hai đều có giá trị, nhưng khác nhau
		LastActor->UnHighLightActor();
		ThisActor->HightLightActor();
	}
	else
	{
		// E. Cả hai đều là cùng một actor -> Không làm gì
	}

}

void AMyPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTag::Get().InputTag_LMB))
	{
		IsTargeting = ThisActor ? true : false;
		IsAutoRunning = false;
	}
	
}

void AMyPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	
	if (!InputTag.MatchesTagExact(FAuraGameplayTag::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagRelease(InputTag);
		}
		return;
	}
	
	if (IsTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagRelease(InputTag);
		}
	}
	else
	{
		APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlledPawn->GetActorLocation(),CachedDestination))
			{
				SplineComponent->ClearSplinePoints();
				for (const auto& PointLocation : NavPath->PathPoints)
				{
					SplineComponent->AddSplinePoint(PointLocation,ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(),PointLocation,8.f,8,FColor::Green,false,5.0f);
				}
				IsAutoRunning = true;
			}
		}
		FollowTime = 0.f;
		IsTargeting = false;
	}
	
}

void AMyPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FAuraGameplayTag::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	if (IsTargeting)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		FHitResult HitResult;
		if (GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,HitResult))
		{
			CachedDestination = HitResult.ImpactPoint;
		}
		if (APawn* ControllerPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControllerPawn->GetActorLocation()).GetSafeNormal();
			ControllerPawn->AddMovementInput(WorldDirection);
		}
	}
}

UAuraAbilitySystemComponent* AMyPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}
