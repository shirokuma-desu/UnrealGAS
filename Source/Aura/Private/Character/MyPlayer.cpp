// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyPlayer.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/MyPlayerController.h"
#include "Player/MyPlayerState.h"
#include "UI/HUD/MyHUD.h"

AMyPlayer::AMyPlayer()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	//set yaw rotation speed
	GetCharacterMovement()->RotationRate = FRotator(0, 400, 0);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
}

void AMyPlayer::InitAbilityActorInfo()
{
	AMyPlayerState* AuraPlayerState = GetPlayerState<AMyPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();

	if (AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(GetController()))
	{
		if (AMyHUD* MyHUD = Cast<AMyHUD>(MyPlayerController->GetHUD()))
		{
			MyHUD->InitOverlayWidget(MyPlayerController,AuraPlayerState,AbilitySystemComponent,AttributeSet);
		}
	}
}

void AMyPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	//init ability actor information for the server
	InitAbilityActorInfo();
}

void AMyPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	//init ability actor information for the client
	InitAbilityActorInfo();
}
