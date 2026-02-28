// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTag.h"
#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interfaces/IInteraction.h"
#include "UI/Widget/DamageTextComponent.h"

AMyPlayerController::AMyPlayerController()
{
	bReplicates = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>("SplineComponent");
}

void AMyPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter,bool bBlockHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass)
	{
		UDamageTextComponent* DamageTextComponent = NewObject<UDamageTextComponent>(TargetCharacter,DamageTextComponentClass);
		DamageTextComponent->RegisterComponent();
		DamageTextComponent->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageTextComponent->SetDamageText(DamageAmount, bBlockHit,bCriticalHit);
	}
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
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Started,this,&AMyPlayerController::OnShiftPressed);
	AuraInputComponent->BindAction(ShiftAction,ETriggerEvent::Completed,this,&AMyPlayerController::OnShiftReleased);
	
	AuraInputComponent->BindAbilityAction(InputConfig,this,&ThisClass::AbilityInputPressed,
		&ThisClass::AbilityInputReleased, &ThisClass::AbilityInputHeld);
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
	
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
	GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility,false,CursorHitResult);
	if (!CursorHitResult.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = CursorHitResult.GetActor();

	if (LastActor != ThisActor)
	{
		if (LastActor) LastActor->UnHighLightActor();
		if (ThisActor) ThisActor->HightLightActor();
	}

}

void AMyPlayerController::AutoRun()
{
	if (!IsAutoRunning) return;
	
		if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = SplineComponent->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		const FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(LocationOnSpline,ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);
		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			IsAutoRunning = false;
		}
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
	
	if (GetASC())
	{
		GetASC()->AbilityInputTagRelease(InputTag);
	}
	
	if (!IsTargeting && !IsShiftDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this,ControlledPawn->GetActorLocation(),CachedDestination))
			{
				SplineComponent->ClearSplinePoints();
				for (const auto& PointLocation : NavPath->PathPoints)
				{
					SplineComponent->AddSplinePoint(PointLocation,ESplineCoordinateSpace::World);
				}
				CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
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
	if (IsTargeting || IsShiftDown)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();
		
		if (CursorHitResult.bBlockingHit)
		{
			CachedDestination = CursorHitResult.ImpactPoint;
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
