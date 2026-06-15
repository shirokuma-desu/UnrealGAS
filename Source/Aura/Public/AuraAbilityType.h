#pragma once
#include "GameplayEffectTypes.h"
#include "AuraAbilityType.generated.h"

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()
	
	FDamageEffectParams(){}
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<class UGameplayEffect> DamagedGameplayEffectClass = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent = nullptr;
	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.0f;
	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamagedType = FGameplayTag();
	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;
	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;
	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;
	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;
	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;
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
	FVector GetDeathImpulse() const {return DeathImpulse;}
	FVector GetKnockbackForce() const {return KnockbackForce;}
	
	void SetCriticalHit(bool IsCritical) { bIsCriticalHit = IsCritical; }
	void SetBlockHit(bool IsBlock) { bIsBlock = IsBlock; }
	void SetSuccessfulDebuff(bool bInIsDebuff) {bIsSuccessfulDebuff = bInIsDebuff;}
	void SetDebuffDamage(float InDebuffDamage) { DebuffDamage = InDebuffDamage;}
	void SetDebuffFrequency(float InDebuffFrequency) { DebuffFrequency = InDebuffFrequency;}
	void SetDebuffDuration(float InDebuffDuration) {DebuffDuration = InDebuffDuration;}
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType) {DamageType = InDamageType;}
	void SetDeathImpulse(const FVector& InImpulse) { DeathImpulse = InImpulse;}
	void SetKnockBackForce(const FVector& InForce) { KnockbackForce = InForce;}
	
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
	
	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;
	
	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;
	
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