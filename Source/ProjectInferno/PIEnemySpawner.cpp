// Fill out your copyright notice in the Description page of Project Settings.

#include "PIEnemySpawner.h"
#include "Engine.h"
#include "ProjectInfernoPatrolEnemy.h"

// Sets default values
APIEnemySpawner::APIEnemySpawner()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APIEnemySpawner::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void APIEnemySpawner::Tick(float delta_time)
{
    Super::Tick(delta_time);

    m_spawn_timer -= delta_time;
    m_death_timer -= delta_time;

    if (m_spawn_timer <= 0)
    {
        SpawnEnemy();
        m_spawn_timer = 5.0f;
    }
    if (m_death_timer <= 0)
    {
        this->Destroy();
    }
}

void APIEnemySpawner::SpawnEnemy()
{
    if (m_enemy)
    {
        auto position = GetActorLocation();
        auto rotation = GetActorRotation();
        auto world = GetWorld();

        if (world)
        {
            FActorSpawnParameters spawn_params;
            spawn_params.Owner = this;
            spawn_params.Instigator = GetInstigator();

            world->SpawnActor<AProjectInfernoPatrolEnemy>(m_enemy, position, rotation, spawn_params);
        }
    }
}

