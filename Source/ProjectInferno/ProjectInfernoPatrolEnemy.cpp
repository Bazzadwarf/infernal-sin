// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectInfernoPatrolEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectInfernoWaypoint.h"
#include "PIWaveController.h"

// Sets default values
AProjectInfernoPatrolEnemy::AProjectInfernoPatrolEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // The character will orient itself to the direction that it is moving in
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 600.0f, 0.0f);

    GetHealthComponent()->SetMaxHealth(100);
    GetHealthComponent()->OnDeath.BindUFunction(this, "OnDeath");
}

void AProjectInfernoPatrolEnemy::BeginPlay()
{
    
    for (TActorIterator<AProjectInfernoWaypoint> waypoints(GetWorld()); waypoints; ++waypoints)
    {
        if (*waypoints != nullptr)
        {
            if (!m_next_waypoint)
            {
                m_next_waypoint = *waypoints;
            }
            else if (this->GetDistanceTo(*waypoints) < this->GetDistanceTo(m_next_waypoint))
            {
                m_next_waypoint = *waypoints;
            }
        }
    }
}

AProjectInfernoWaypoint* AProjectInfernoPatrolEnemy::GetNextWaypoint()
{
    return m_next_waypoint;
}

void AProjectInfernoPatrolEnemy::SetNextWaypoint(AProjectInfernoWaypoint* next_waypoint)
{
    m_next_waypoint = next_waypoint;
}

float AProjectInfernoPatrolEnemy::GetHealth()
{
    return GetHealthComponent()->GetCurrentHealth();
}

float AProjectInfernoPatrolEnemy::GetHealthPercent()
{
    return GetHealth() / GetHealthComponent()->GetMaxHealth();
}

void AProjectInfernoPatrolEnemy::OnDeath()
{
    if (Cast<APIWaveController>(GetOwner()))
    {
        Cast<APIWaveController>(GetOwner())->EnemyDied();
    }

    this->Destroy();
}
