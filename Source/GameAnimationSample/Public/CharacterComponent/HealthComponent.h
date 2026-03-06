// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"
class UBaseCharacterData;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEANIMATIONSAMPLE_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();
	void SetupHealthComponent(UBaseCharacterData* BCD);
	void SetupMaxHealth();
	void UpdateHealthByDamage(float Damage);
	UPROPERTY()
	UBaseCharacterData* BaseCharacterData;
	UPROPERTY()
	class ABaseCharacter* Character;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float CurrentHealth ;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


		
};
