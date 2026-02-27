#include "AuraAbilityType.h"

UScriptStruct* FAuraGameplayEffectContext::GetScriptStruct() const
{
	return FGameplayEffectContext::GetScriptStruct();
}

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	return FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);
}
