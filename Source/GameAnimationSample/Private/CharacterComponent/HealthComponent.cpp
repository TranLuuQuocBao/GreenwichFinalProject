// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterComponent/HealthComponent.h"
#include "DataAsset/BaseCharacterData.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;


}

void UHealthComponent::SetupHealthComponent(UBaseCharacterData* BCD)
{
	BaseCharacterData = BCD;

}


void UHealthComponent::SetupMaxHealth()
{
	MaxHealth = BaseCharacterData->MaxHealth;
	CurrentHealth = BaseCharacterData->CurrentHealth;
}

void UHealthComponent::UpdateHealthByDamage(float Damage)
{
	CurrentHealth = FMath::Max(CurrentHealth - Damage, 0.0f);
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


