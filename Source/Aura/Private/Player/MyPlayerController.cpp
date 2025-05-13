// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interfaces/IInteraction.h"

AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;
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
	check(Subsytem);
	//init IMC
	Subsytem->AddMappingContext(PlayerInputMappingContext, 0);
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
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AMyPlayerController::Move);
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

	if (APawn* ControllerPawn = GetPawn<APawn>())
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
