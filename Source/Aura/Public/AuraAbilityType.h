#pragma once
#include "GameplayEffectTypes.h"
#include "AuraAbilityType.generated.h"

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{

	GENERATED_BODY()
public:
	
	bool IsCriticalHit() const {return bIsCriticalHit;}
	bool IsBlockHit() const {return bIsBlock;}
	
	void SetCriticalHit(bool IsCritical) { bIsCriticalHit = IsCritical; }
	void SetBlockHit(bool IsBlock) { bIsBlock = IsBlock; }
	
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