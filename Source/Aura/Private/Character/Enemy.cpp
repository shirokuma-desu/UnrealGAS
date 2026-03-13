// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTag.h"
#include "AbilitySystem/AuraAbilitySystemBPLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AI/AuraAIController.h"
#include "Aura/Aura.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/Widget/AuraUserWidget.h"

AEnemy::AEnemy()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera,ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile,ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);
	
	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("Aura_AbilitySystem_Component");
	HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBarWidget");
	HealthBar->SetupAttachment(GetRootComponent());
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("Aura_AttributeSet");
	
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
	InitAbilityActorInfo();
	if (HasAuthority())
	{
		UAuraAbilitySystemBPLibrary::GiveStartupAbilities(this, AbilitySystemComponent,ECharacterClass);
	}
	
	
 	if ( UAuraUserWidget* AuraUserWidget =  Cast<UAuraUserWidget>(HealthBar->GetUserWidgetObject()))
 	{
 		AuraUserWidget->SetWidgetController(this);
 	}

	if (const UAuraAttributeSet* AuraAS = Cast<UAuraAttributeSet>(AttributeSet))
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChange.Broadcast(Data.NewValue);
			}
			);
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAS->GetMaxManaAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnMaxHealthChange.Broadcast(Data.NewValue);
			}
			);
			OnHealthChange.Broadcast(AuraAS->GetHealth());
			OnMaxHealthChange.Broadcast(AuraAS->GetMaxHealth());
	}
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTag::Get().HitReact,EGameplayTagEventType::NewOrRemoved).AddUObject(
	this,
	&AEnemy::HitReactTagChanged
	);
	
}

void AEnemy::InitAbilityActorInfo()
{
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->AbilityActorInfoSet();
	if (HasAuthority())
	{
		InitializeDefaultAttributes();
	}
}

void AEnemy::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemBPLibrary::InitializeDefaultAttributes(this,ECharacterClass,Level,AbilitySystemComponent);
}

void AEnemy::HighLightActor()
{
	OnHoverHappened.Broadcast();
}

void AEnemy::UnHighLightActor()
{
	OnNotHoverEnemyHappened.Broadcast();
}

void AEnemy::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	CombatTarget = InCombatTarget;
}

AActor* AEnemy::GetCombatTarget_Implementation() const
{
	return  CombatTarget;
}

int32 AEnemy::GetPlayerLevel()
{
	return Level;
}

void AEnemy::Die()
{
	SetLifeSpan(LifeSpan);
	if (AuraAIController)
	{
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("Dead"),true);
		
	}
	Super::Die();
}

void AEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!HasAuthority()) return;
	
	AuraAIController = Cast<AAuraAIController>(NewController);
	
	AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	AuraAIController->RunBehaviorTree(BehaviorTree);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"),false);
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangeAttacker"),ECharacterClass  != ECharacterClass::Warrior);
	
}

void AEnemy::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	bHitReacting = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bHitReacting ? 0.f : BaseWalkSpeed;
	if (AuraAIController &&AuraAIController->GetBlackboardComponent() )
	{
	AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"),bHitReacting);
	}
}

