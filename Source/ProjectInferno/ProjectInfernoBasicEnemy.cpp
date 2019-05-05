// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectInfernoBasicEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"
#include "ProjectInfernoBasicEnemyController.h"

// Sets default values
AProjectInfernoBasicEnemy::AProjectInfernoBasicEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;

    // Initialise sensing component
    m_pawn_sensing_component = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

    // Set vision field of view to 90 degrees
    m_pawn_sensing_component->SetPeripheralVisionAngle(85.f);
    m_pawn_sensing_component->SetSensingUpdatesEnabled(true);
    m_pawn_sensing_component->SensingInterval = 0.25f;
}

// Called when the game starts or when spawned
void AProjectInfernoBasicEnemy::BeginPlay()
{
    Super::BeginPlay();

    // Register AIController function OnSeePlayer
    if (m_pawn_sensing_component)
    {
        m_pawn_sensing_component->OnSeePawn.AddDynamic(this, &AProjectInfernoBasicEnemy::OnSeePlayer);
    }
}

void AProjectInfernoBasicEnemy::OnSeePlayer(APawn* pawn)
{
    // Check if pawn sensed is of type AProjectInfernoBasicEnemyController
    if (auto ai_controller = GetController<AProjectInfernoBasicEnemyController>())
    {
        // Set the seen target on the blackboard
        if (m_pawn_sensing_component->HasLineOfSightTo(pawn))
        {
            ai_controller->SetSeenTarget(pawn);
        }
    }

    GetWorldTimerManager().SetTimer(m_timer_handle, this, &AProjectInfernoBasicEnemy::OnLosePlayer, 0.3f, false);
}

void AProjectInfernoBasicEnemy::OnLosePlayer()
{
    // Check if pawn sensed is of type AProjectInfernoBasicEnemyController
    if (auto ai_controller = GetController<AProjectInfernoBasicEnemyController>())
    {
        ai_controller->SetSeenTarget(nullptr);
    }
}
