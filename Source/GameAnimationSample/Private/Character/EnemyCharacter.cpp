// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"
#include "Interface/CombatInterface.h"
FVector AEnemyCharacter::I_GetPatrolLocation()
{
	if (PatrolPoints.IsEmpty()) return GetActorLocation();
	if (!PatrolPoints[PatrolIndex]) return GetActorLocation();
	auto ReturnValue = PatrolPoints[PatrolIndex]->GetActorLocation();

	PatrolIndex = (PatrolIndex + 1) % PatrolPoints.Num() ;
	return ReturnValue;
}

void AEnemyCharacter::I_HandleSeePlayer(AActor* PlayerActor)
{
	CombatInterface = TScriptInterface<ICombatInterface>(PlayerActor);
	CombatInterface->I_EnterCombat(HealthComponent->CurrentHealth, HealthComponent->MaxHealth);
}

void AEnemyCharacter::Destroyed()
{
	Super::Destroyed();
	CombatInterface->I_HandleTagetDestroyed();


}

void AEnemyCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	Super::HandleTakePointDamage(DamagedActor, Damage, InstigatedBy, HitLocation, FHitComponent, BoneName, ShotFromDirection, DamageType, DamageCauser);
	CombatInterface->I_HitTarget(HealthComponent->CurrentHealth, HealthComponent->MaxHealth);
}

void AEnemyCharacter::HandleDead()
{
	Super::HandleDead();
	DetachFromControllerPendingDestroy();

}
