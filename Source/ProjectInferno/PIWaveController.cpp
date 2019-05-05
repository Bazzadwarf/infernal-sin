// Fill out your copyright notice in the Description page of Project Settings.

#include "PIWaveController.h"

APIWaveController::APIWaveController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APIWaveController::BeginPlay()
{
    Super::BeginPlay();
}

void APIWaveController::Tick(float delta_time)
{
    Super::Tick(delta_time);

    if (m_num_enemies_left == 0)
    {
        SpawnNextWave();
        m_wave_index++;
    }

    if (m_wave_index == m_waves.Num())
    {
        for (auto spawner : m_spawners)
        {
            if (spawner.enemySpawner)
            {
                spawner.enemySpawner->DestroySelf();
            }
        }

        this->Destroy();
    }
}

void APIWaveController::NotifyActorBeginOverlap(AActor* actor)
{
    Super::NotifyActorBeginOverlap(actor);

    if (m_wave_index == 0 && actor == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
    {
        m_num_enemies_left = 0;
        SpawnNextWave();
        m_wave_index++;
    }
}

void APIWaveController::SpawnNextWave()
{
    if (m_spawners.Num() != 0)
    {
        for (auto spawner : m_spawners)
        {
            if (!spawner.enemySpawner)
            {
                return;
            }
        }

        auto nextWave = m_waves[m_wave_index];

        for (FPIEnemyStruct enemy : m_waves[m_wave_index].enemies)
        {
            for (size_t i = 0; i < enemy.num_to_spawn; i++)
            {
                m_spawners[0].enemySpawner->SpawnEnemy(enemy.enemy_type, this);
                m_spawners.Sort([](const FPISpawnerStruct& LHS, const FPISpawnerStruct& RHS) {
                    return LHS.timersPicked < RHS.timersPicked;
                });
                m_num_enemies_left++;
            }
        }
    }
}

void APIWaveController::EnemyDied()
{
    m_num_enemies_left--;
}
