#pragma once
#include "GameplayEffectTypes.h"
#include "AuraAbilityType.generated.h"

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	
	FDamageEffectParams(){}
	
	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;
	
	UPROPERTY()
	TSubclassOf<class UGameplayEffect> DamagedGameplayEffectClass = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;

	float BaseDamage = 0.0f;
	float AbilityLevel = 1.f;
	
	FGameplayTag DamagedType = FGameplayTag();
	
	float DebuffChance = 0.f;
	float DebuffDamage = 0.f;
	float DebuffFrequency = 0.f;
	float DebuffDuration = 0.f;
	
};


USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{

	GENERATED_BODY()
public:
	
	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsBlockHit() const {return bIsBlock;}
	bool IsSuccessfulDebuff() const { return bIsSuccessfulDebuff;}
	float GetDebuffDamage() const { return  DebuffDamage;}
	float GetDebuffDuration()const { return  DebuffDuration;}
	float GetDebuffFrequency() const {return  DebuffFrequency;}
	TSharedPtr<FGameplayTag> GetDamageType() const {return  DamageType;}
	
	
	void SetCriticalHit(bool IsCritical) { bIsCriticalHit = IsCritical; }
	void SetBlockHit(bool IsBlock) { bIsBlock = IsBlock; }
	void SetSuccessfulDebuff(bool bInIsDebuff) {bIsSuccessfulDebuff = bInIsDebuff;}
	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage;}
	void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency;}
	void SetDebuffDuration(float InDebuffDuration) {DebuffDuration = InDebuffDuration;}
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) {DamageType = InDamageType;}
	virtual  UScriptStruct* GetScriptStruct() const override;
	virtual  bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;
	virtual FAuraGameplayEffectContext* Duplicate() const override;
	
protected:
	
	UPROPERTY()
	bool bIsBlock = false;
	
	UPROPERTY()
	bool bIsCriticalHit = false;
	UPROPERTY()
	uint32 RepBits = 0;
	
	UPROPERTY()
	bool bIsSuccessfulDebuff = false;
	UPROPERTY()
	float DebuffDamage= 0.f;
	UPROPERTY()
	float DebuffDuration = 0.f;
	UPROPERTY()
	float DebuffFrequency = 0.f;
	
	
	TSharedPtr<FGameplayTag> DamageType;
	
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};