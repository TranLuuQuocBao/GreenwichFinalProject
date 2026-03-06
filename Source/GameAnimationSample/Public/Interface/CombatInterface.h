// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GAMEANIMATIONSAMPLE_API ICombatInterface
{
	GENERATED_BODY()
public:
	virtual void I_PickupMontage(UAnimMontage* PickupMontage) = 0; 
	virtual void I_EnterCombat(float EnemyHealth, float EnemyMaxhealth);
	virtual void I_HitTarget(float HealthTarget, float MaxhealthTarget);
	UFUNCTION(BlueprintCallable)
	virtual void I_RequestAttack() = 0;
	virtual void I_HandleTagetDestroyed();
};
