// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Interface/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class GAMEANIMATIONSAMPLE_API AEnemyCharacter : public ABaseCharacter, public IEnemyInterface
{
	GENERATED_BODY()
public:
	virtual FVector I_GetPatrolLocation() override;
	virtual void I_HandleSeePlayer(AActor* PlayerActor) override;
	virtual void Destroyed() override;
protected:
	virtual void HandleTakePointDamage(
		AActor* DamagedActor,
		float Damage,
		AController* InstigatedBy,
		FVector HitLocation,
		UPrimitiveComponent* FHitComponent,
		FName BoneName,
		FVector ShotFromDirection,
		const UDamageType* DamageType,
		AActor* DamageCauser) override;

	virtual void HandleDead() override;
private:
	UPROPERTY(EditInstanceOnly)
	TArray<AActor*> PatrolPoints;

	UPROPERTY()
	TScriptInterface<ICombatInterface> CombatInterface;
	int PatrolIndex = 0;

};
