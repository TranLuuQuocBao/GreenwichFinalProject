// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/EnemyAIController.h"
#include "Interface/EnemyInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h" 
AEnemyAIController::AEnemyAIController()
{
	AIPerceptionComponent =  CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AI Perception Component"));

	AISightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("AI Sense Config"));
	AISightConfig->SightRadius = 2500.0f;
	AISightConfig->LoseSightRadius = 2500.0f;
	AISightConfig->PeripheralVisionAngleDegrees = 55.0f;
	AISightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerceptionComponent->ConfigureSense(*AISightConfig);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	EnemyInterface = TScriptInterface<IEnemyInterface>(InPawn);
	RunBehaviorTree(BehaviorTree);
	if(AIPerceptionComponent)
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::HandleTagetPerceptionUpdate);
}

void AEnemyAIController::HandleTagetPerceptionUpdate(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed())
	{
		Blackboard->SetValueAsBool(Key_IsCombat, true);
		Blackboard->SetValueAsObject(Key_PlayerActor, Actor);
	}
	else
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Lose Player"));
	}
	if (EnemyInterface)
		EnemyInterface->I_HandleSeePlayer(Actor);
}



void AEnemyAIController::UpdatePatrolLocation()
{

	if (Blackboard)
	Blackboard->SetValueAsVector(Key_PatrolLocation, EnemyInterface->I_GetPatrolLocation());
}

void AEnemyAIController::CheckDistance(AActor* AIActor, AActor* PlayerActor, float AttackRange)
{
	if (!AIActor) return;
	const auto DistanceAIActor= AIActor->GetDistanceTo(PlayerActor);

	if (DistanceAIActor <= AttackRange) 
	{
		Blackboard->SetValueAsBool(Key_ShouldAttack, true);
	}
	else
	{
		Blackboard->SetValueAsBool(Key_ShouldAttack, false);
	}
}
