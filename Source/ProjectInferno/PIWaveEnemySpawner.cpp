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

            UE_LOG(LogTemp, Warning, TEXT("%s"), *GetNameSafe(this));
        }
    }
}

void APIWaveEnemySpawner::DestroySelf()
{
    this->Destroy();
}
