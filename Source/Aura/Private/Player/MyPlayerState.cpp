// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/MyPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AMyPlayerState::AMyPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("Aura_AbilitySystem_Component");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("Aura_AttributeSet");
	NetUpdateFrequency = 100.f;
	
}

void AMyPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyPlayerState, Level);
	DOREPLIFETIME(AMyPlayerState, XP);
	DOREPLIFETIME(AMyPlayerState, AttributePoints);
	DOREPLIFETIME(AMyPlayerState, SpellPoints);
}

UAbilitySystemComponent* AMyPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMyPlayerState::AddToXp(int32 InXP)
{
	XP+=InXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void AMyPlayerState::SetXP(int32 InXP)
{
	XP	= InXP;
	OnXPChangeDelegate.Broadcast(XP);
}

void AMyPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AMyPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangeDelegate.Broadcast(Level);
}

void AMyPlayerState::AddToAttributePoints(int32 InAttributePoints)
{
	AttributePoints+=InAttributePoints;
	OnAttributePointsChangeDelegate.Broadcast(AttributePoints);
}

void AMyPlayerState::AddToSpellPoints(int32 InSpellPoints)
{
	SpellPoints+=InSpellPoints;
	OnSpellPointsChangeDelegate.Broadcast(SpellPoints);
}

void AMyPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangeDelegate.Broadcast(Level);
}

void AMyPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangeDelegate.Broadcast(XP);
}

void AMyPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangeDelegate.Broadcast(AttributePoints);
}


void AMyPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangeDelegate.Broadcast(SpellPoints);
}
