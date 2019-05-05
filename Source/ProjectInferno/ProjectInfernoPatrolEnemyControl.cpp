// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectInfernoPatrolEnemyControl.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ProjectInfernoPatrolEnemy.h"
#include "ProjectInfernoPlayerCharacter.h"
#include "ProjectInfernoProjectile.h"
#include "ProjectInfernoWaypoint.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

AProjectInfernoPatrolEnemyControl::AProjectInfernoPatrolEnemyControl()
{
    // Set this character to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;

    // Initialise sight config
    m_sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));

    // Initialise perception component
    SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

    // Configure sight options
    m_sight_config->SightRadius = m_detection_radius;
    m_sight_config->LoseSightRadius = m_undetect_radius;
    m_sight_config->PeripheralVisionAngleDegrees = m_peripheral_vision_angle;
    m_sight_config->SetMaxAge(m_detection_age);

    m_sight_config->DetectionByAffiliation.bDetectEnemies = true;
    m_sight_config->DetectionByAffiliation.bDetectFriendlies = true;
    m_sight_config->DetectionByAffiliation.bDetectNeutrals = true;

    // Set dominant sense as sight
    GetPerceptionComponent()->SetDominantSense(*m_sight_config->GetSenseImplementation());
    GetPerceptionComponent()->ConfigureSense(*m_sight_config);

    // When a pawn is detected
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &AProjectInfernoPatrolEnemyControl::PawnDetected);
}

void AProjectInfernoPatrolEnemyControl::Tick(float delta_time)
{
    Super::Tick(delta_time);

    auto enemy_patrol = Cast<AProjectInfernoPatrolEnemy>(GetPawn());

    m_time_to_fire -= delta_time;

    if (m_time_to_fire <= 0)
    {
        Fire();
        m_time_to_fire = 3.0f;
    }

    // If the player leaves the enemies detection radius
    if (m_distance_to_player > m_detection_radius)
    {
        m_player_detected = false;
    }

    // If there is a waypoint to move to, AND the player is not detected
    if (enemy_patrol->GetNextWaypoint() != nullptr && m_player_detected == false)
    {
        // Move to enemy patrollers next waypoint, stopping at 5.0f away from it
        MoveToActor(enemy_patrol->GetNextWaypoint(), 5.0f);
    }
    else if (m_player_detected == true)
    {
        MoveToActor(m_target, 5.0f);
    }
}

FRotator AProjectInfernoPatrolEnemyControl::GetControlRotation() const
{
    if (GetPawn() == nullptr)
    {
        return {0.0f, 0.0f, 0.0f};
    }

    return {0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f};
}

void AProjectInfernoPatrolEnemyControl::PawnDetected(const TArray<AActor*>& pawns)
{
    for (auto pawn : pawns)
    {
        if (auto player = Cast<AProjectInfernoPlayerCharacter>(pawn))
        {
            m_distance_to_player = GetPawn()->GetDistanceTo(pawn);

            m_player_detected = true;
            m_target = player;
        }
    }
}

void AProjectInfernoPatrolEnemyControl::Fire()
{
    if (m_projectile && m_player_detected == true)
    {
        auto enemy_position = GetPawn()->GetActorLocation();
        auto enemy_rotation = GetControlRotation();
        auto world = GetWorld();

        if (world)
        {
            FActorSpawnParameters spawn_params;
            spawn_params.Owner = this;
            spawn_params.Instigator = Instigator;

            auto projectile = world->SpawnActor<AProjectInfernoProjectile>(
                m_projectile, enemy_position, enemy_rotation, spawn_params);

            if (projectile && m_target)
            {
                auto direction = (m_target->GetActorLocation() - GetPawn()->GetActorLocation()).GetSafeNormal();
                float random_deviation = rand() % 6 + (-5);
                direction.Y += (random_deviation / 10);
                projectile->FireInDirection(direction);
            }
            else if (projectile)
            {
                projectile->FireInDirection(enemy_rotation.Vector());
            }
        }
    }
}
