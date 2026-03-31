// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ModMagCalc/MMC_MaxMana.h"

#include "AbilitySystem/AuraAttributeSet.h"
#include "Interfaces/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.bSnapshot = false;
	
	RelevantAttributesToCapture.Add(UMMC_MaxMana::IntelligenceDef);  
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
		//Gather GameplayTag from source and target
    	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
    	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
    	
    	FAggregatorEvaluateParameters EvaluateParameters;
    	EvaluateParameters.SourceTags = SourceTag;
    	EvaluateParameters.TargetTags = TargetTag;
    	
    	float Intel = 0;
    	
    	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluateParameters, Intel);
    	Intel = FMath::Max<float>(Intel, 0.0f);
    	int32	PlayerLevel = 1;
		if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
		{
			PlayerLevel = ICombatInterface::Execute_GetPlayerLevel((Spec.GetContext().GetSourceObject()));
		}
    	
    	return 50.f + 3.0f + 10.f * PlayerLevel;
}
