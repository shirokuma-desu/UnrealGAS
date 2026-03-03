// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityType.h"
#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAbilitySystemBPLibrary.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interfaces/CombatInterface.h"

struct  AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPen);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritDmg);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CritResist);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightingResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,Armor,Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,BlockChance,Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArmorPen, Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CritHitChance, Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CritDmg, Source,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,CritResist, Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,FireResistance, Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,ArcaneResistance, Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,LightingResistance, Target,false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet,PhysicalResistance, Target,false);
		
		const FAuraGameplayTag& Tags = FAuraGameplayTag::Get();
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_Armor, ArmorDef); 
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_BlockChance, BlockChanceDef); 
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_ArmorPen, ArmorPenDef); 
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CritHitChance, CritHitChanceDef); 
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CritDmg, CritDmgDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Secondary_CritResist, CritResistDef);
		
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Fire, FireResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Arcane, ArcaneResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Lighting, LightingResistanceDef);
		TagsToCaptureDefs.Add(Tags.Attributes_Resistance_Physical, PhysicalResistanceDef);
		
		
	}
	
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritDmgDef);
	RelevantAttributesToCapture.Add(DamageStatics().CritResistDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	
	ICombatInterface*  SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface*  TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTag;
	EvaluateParameters.TargetTags = TargetTag;
	
	//get Damage set by caller magnitude
	float Damage = 0.f;
	for (const TPair<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTag::Get().DamageTypesToResistance)
	{
		const FGameplayTag DamageTypeTag = Pair.Key;
		const FGameplayTag ResistanceTypeTag = Pair.Value;
		checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceTypeTag), TEXT("TagToDefs doesnt contain tag [%s] in ExecCalc"), *ResistanceTypeTag.ToString());
		
		const FGameplayEffectAttributeCaptureDefinition CaptureDefinition = AuraDamageStatics().TagsToCaptureDefs[ResistanceTypeTag];
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDefinition,EvaluateParameters,Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f,100.f);
		
		DamageTypeValue *= (100.f - Resistance) / 100.f;
		Damage += DamageTypeValue;
	}
	
	//Capture block chance on target
	float TargetBlockChance = 0;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluateParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.0f);
	
	const bool bBlocked = FMath::RandRange(1,100) < TargetBlockChance;
	
	FGameplayEffectContextHandle EffectContextHandle = Spec.GetEffectContext();
	UAuraAbilitySystemBPLibrary::SetIsBlockHit(EffectContextHandle,bBlocked);
	
	bBlocked ? Damage*=0.5f : Damage;
	
	
	float SourceCritHitChance = 0.f;
	float SourceCritDmg = 0.f;
	float TargetCritResist = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritHitChanceDef,EvaluateParameters, SourceCritHitChance);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritDmgDef,EvaluateParameters, SourceCritDmg);
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CritResistDef,EvaluateParameters, TargetCritResist);
	
	SourceCritHitChance = FMath::Max<float>(SourceCritHitChance,0.f);
	SourceCritDmg = FMath::Max<float>(SourceCritDmg,0.f);
	TargetCritResist = FMath::Max<float>(TargetCritResist,0.f);
	
	//ArmorPen ignores a percentage of the target's armor
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef,EvaluateParameters,TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor,0.f);
	
	float SourceArmorPen = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenDef,EvaluateParameters,SourceArmorPen);
	SourceArmorPen = FMath::Max<float>(SourceArmorPen,0.f);
	
	//Get Curve Value and use it  <----
	const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemBPLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("ArmorPen"),FString());
	
	const float ArmorPenCoefficient  = ArmorPenCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	
	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("EffectiveArmor"),FString());
	const float EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	//---------->
	
	const FRealCurve* CritResistCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName("CriticalResist"),FString());
	const float CritResistCoefficient = CritResistCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	const float EffectiveCritHitChance = SourceCritHitChance - TargetCritResist * CritResistCoefficient;
	
	// Critical hit resit reduce critical hit chance by a percent
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPen * ArmorPenCoefficient )/100.f;
	Damage*= (100 - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;
	
	//Double damage plus a bonus if critical
	const bool bCritical = FMath::RandRange(1,100) <  EffectiveCritHitChance;
	
	UAuraAbilitySystemBPLibrary::SetIsCriticalHit(EffectContextHandle, bCritical);
	
	Damage = bCritical ? Damage *= 2.0f + SourceCritDmg : Damage;
	
	
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncommingDamageAttribute(),EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
}
