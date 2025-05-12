// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MyPlayer.h"

#include "GameFramework/CharacterMovementComponent.h"

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
