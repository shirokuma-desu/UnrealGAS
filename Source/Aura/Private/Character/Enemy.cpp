// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"

void AEnemy::HightLightActor()
{
	OnHoverHappened.Broadcast();
}

void AEnemy::UnHighLightActor()
{
	OnNotHoverEnemyHappened.Broadcast();
}
