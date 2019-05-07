// Fill out your copyright notice in the Description page of Project Settings.

#include "PIWaveEnemySpawner.h"
#include "Engine.h"
#include "PIWaveController.h"
#include "ProjectInfernoPatrolEnemy.h"

void APIWaveEnemySpawner::SpawnEnemy(TSubclassOf<AProjectInfernoPatrolEnemy> EnemyType, APIWaveController* parent)
{
    if (EnemyType)
    {
        auto position = GetActorLocation();
        auto rotation = GetActorRotation();
        auto world = GetWorld();

        if (world)
        {
            FActorSpawnParameters spawn_params;
            spawn_params.Owner = parent;
            spawn_params.Instigator = Instigator;

            // CHANGE TO NEW AI CLASS
            world->SpawnActor<AProjectInfernoPatrolEnemy>(EnemyType, position, rotation, spawn_params);

            if (m_on_spawn_particle_effect != nullptr)
            {
                UGameplayStatics::SpawnEmitterAtLocation(this,
                                                         m_on_spawn_particle_effect,
                                                         this->ActorToWorld().GetLocation() + FVector(0.0f, 10.0f, 0.0f),
                                                         FRotator(90.0f, 0.0f, 0.0f),
                                                         true);
            }

            if (m_on_spawn_sound != nullptr)
            {
                UGameplayStatics::SpawnSoundAtLocation(this,
                                                       m_on_spawn_sound,
                                                       this->ActorToWorld().GetLocation(),
                                                       FRotator::ZeroRotator,
                                                       1.0f,
                                                       1.0f,
                                                       0.0f,
                                                       nullptr,
                                                       nullptr,
                                                       true);
            }

        }
    }
}

void APIWaveEnemySpawner::DestroySelf()
{
    this->Destroy();
}
