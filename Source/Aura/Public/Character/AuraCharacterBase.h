// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"
#include "NiagaraSystem.h"
#include "GameFramework/Character.h"
#include "Interfaces/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAuraGameplayAbility;
class UAbilitySystemComponent;
class UAttributeSet;
class UAnimMontage;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	
	AAuraCharacterBase();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//* IAbilitySystemInterface *//
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	//combat interface
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual	void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatarActor_Implementation() override;
	virtual TArray<FTaggedMontage> GetAllAttackMontage_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() override;
	
	UFUNCTION(NetMulticast, Reliable)
	virtual  void MC_HandleDeath();
	
	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackMontages;
	
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
protected:
	
	
	
	//var
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category= "Mesh| Weapon", BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryPrimaryAttributes;

	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	FName WeaponTipSocketName;

	UPROPERTY(EditAnywhere,Category="Combat")
	FName LeftHandSocket;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	FName RightHandSocket;
	
	UPROPERTY(EditAnywhere,Category="Combat")
	FName TailSocket;
	
	
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float level) const;
	
	virtual void InitializeDefaultAttributes() const;
	
	void AddCharacterAbilities() const;
	
	void Dissolve();
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimelineMesh(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimelineWeapon(UMaterialInstanceDynamic* DynamicMaterialInstance);
	
	/* Dissolve Effect*/
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstanceMesh;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstanceWeapon;
	
	bool bDead = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* BloodEffect;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = "Combat")
	USoundBase* DeadSound; 
	
	//Minions
	int32 MinionCount = 0;
	
private:
	
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
	UPROPERTY(EditAnywhere, Category= "Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
