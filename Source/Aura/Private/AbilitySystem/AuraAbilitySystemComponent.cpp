// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAbilitySystemBPLibrary.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Aura/AuraLogChannels.h"
#include "Interfaces/PlayerInterface.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbility(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	
	
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		if (const UAuraGameplayAbility* AuraGameplayAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraGameplayAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTag::Get().Ability_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}
	
	bStartupAbilityGiven = true;
	OnAbilityGivenHandler.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbility(
	const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility> AbilityClass : StartupPassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}	
}

void UAuraAbilitySystemComponent::AbilityInputTagRelease(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag) && AbilitySpec.IsActive())
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			}
		}
	}	
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock	ActiveScopeLock(*this);
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogAura,Error, TEXT("Failed to Execute delegate in %hs"), __FUNCTION__)
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (FGameplayTag Tag: AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag Tag: AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (FGameplayTag StatusTag: AbilitySpec.DynamicAbilityTags)
	{
		if (StatusTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities.Status"))))
		{
			return StatusTag;
		}
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			ROS_UpgradeAttribute(AttributeTag);
		}
	}
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		for (FGameplayTag Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}
	return  nullptr;
}

bool UAuraAbilitySystemComponent::GetDescriptionByAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription,
	FString& OutNextLevelDescription)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (UAuraGameplayAbility* AuraGameplayAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = AuraGameplayAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraGameplayAbility->GetNextLevelDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemBPLibrary::GetAbilityInfo(GetAvatarActor());
	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FAuraGameplayTag::Get().Ability_None))
	{
		OutDescription = FString();
	}
	else
	{
		OutDescription = UAuraGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoByTag(AbilityTag).LevelRequirement);
	}
	OutNextLevelDescription = FString();
	return false;
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return  GetStatusFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*AbilitySpec);
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::ClearSlot( FGameplayAbilitySpec* AbilitySpec)
{
	const FGameplayTag Slot = GetInputTagFromSpec(*AbilitySpec);
	AbilitySpec->DynamicAbilityTags.RemoveTag(Slot);
	MarkAbilitySpecDirty(*AbilitySpec);
}

void UAuraAbilitySystemComponent::ClearAbilityOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopedAbilityListLockLock(*this);
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&AbilitySpec, Slot))
		{
			ClearSlot(&AbilitySpec);
			
		}
	}
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& Slot)
{
	for (FGameplayTag Tag : Spec->DynamicAbilityTags)
	{
		if (Tag.MatchesTag(Slot))
		{
			return true;
		}
	}
	return false;
}

void UAuraAbilitySystemComponent::ROS_SpendSpellPoints_Implementation(const FGameplayTag& AbilityTag)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoint(GetAvatarActor(),-1);
		}
		
		const FAuraGameplayTag GameplayTag = FAuraGameplayTag::Get();
		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);
		if (Status.MatchesTagExact(GameplayTag.Ability_Status_Eligible))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTag.Ability_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTag.Ability_Status_Unlocked);
			Status = GameplayTag.Ability_Status_Unlocked;
		}
		else if (Status.MatchesTagExact(GameplayTag.Ability_Status_Equipped) || Status.MatchesTagExact(GameplayTag.Ability_Status_Unlocked)) 
		{
			AbilitySpec->Level += 1;
		}
		ROC_UpdateAbilityStatus(AbilityTag,Status,AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::ROS_EquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& Slot)
{
	if (FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FAuraGameplayTag GameplayTag = FAuraGameplayTag::Get();
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusFromSpec(*AbilitySpec);
		
		const bool bStatusValid = Status  == GameplayTag.Ability_Status_Equipped || Status ==GameplayTag.Ability_Status_Unlocked;
		if (bStatusValid)
		{
			//Remove this input tag (slot) from Ability has it.
			ClearAbilityOfSlot(Slot);
			//Clear this ability slot, just in case it a different slot
			ClearSlot(AbilitySpec);
			//Now Assign this slot to this ability
			AbilitySpec->DynamicAbilityTags.AddTag(Slot);
			if (Status.MatchesTagExact(GameplayTag.Ability_Status_Unlocked))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTag.Ability_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(GameplayTag.Ability_Status_Equipped);
			}
			MarkAbilitySpecDirty(*AbilitySpec);
			ROC_EquipAbility(AbilityTag,GameplayTag.Ability_Status_Equipped,Slot,PrevSlot);
		}
		
	}
}

void UAuraAbilitySystemComponent::ROC_EquipAbility_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot)
{
	AbilityEquipped.Broadcast(AbilityTag,Status,Slot,PrevSlot);
}

void UAuraAbilitySystemComponent::UpdateAbilityStatus(int32 Level)
{
	UAbilityInfo* AbilityInfo = UAuraAbilitySystemBPLibrary::GetAbilityInfo(GetAvatarActor());
	for (const FAuraAbilityInfo& Info : AbilityInfo->AbilitiesInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
			
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Info.AbilityClass,1);
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTag::Get().Ability_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			ROC_UpdateAbilityStatus(Info.AbilityTag,FAuraGameplayTag::Get().Ability_Status_Eligible,1);
		}
	}
}

void UAuraAbilitySystemComponent::ROS_UpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(),AttributeTag,Payload);
	if (GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoint(GetAvatarActor(), -1);
	}
	
}

void UAuraAbilitySystemComponent::ROC_UpdateAbilityStatus_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag,int32 AbilityLevel)
{
	OnAbilityStatusChangedHandler.Broadcast(AbilityTag, StatusTag,AbilityLevel);
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	if (!bStartupAbilityGiven)
	{
		bStartupAbilityGiven = true;
		OnAbilityGivenHandler.Broadcast();
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle) const
{
	FGameplayTagContainer Tags;
	GameplayEffectSpec.GetAllAssetTags(Tags);

	//fire an event to overlay controller to catch
	OnEffectAssetTagsHandler.Broadcast(Tags);
}

