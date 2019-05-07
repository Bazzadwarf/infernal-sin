// Fill out your copyright notice in the Description page of Project Settings.

#include "PITeleporter.h"

APITeleporter::APITeleporter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APITeleporter::BeginPlay()
{
    Super::BeginPlay();
}

void APITeleporter::Tick(float delta_time)
{
    Super::Tick(delta_time);

    if (GetIsSpawning())
    {
        m_particle_timer += delta_time;
        if (m_particle_timer >= 3)
        {
            m_particle_timer = 0;
            SetIsSpawning(false);
        }
    }
}

void APITeleporter::SpawnLightAdd()
{
    FActorSpawnParameters spawn_params;
    spawn_params.Owner = this;

    if (m_light_add)
    {
        SetIsSpawning(true);
        GetWorld()->SpawnActor<AProjectInfernoPatrolEnemy>(m_light_add, this->GetActorLocation(), GetPlayerDirection(), spawn_params);
    }
}

void APITeleporter::SpawnRangedAdd()
{
    FActorSpawnParameters spawn_params;
    spawn_params.Owner = this;

    if (m_ranged_add)
    {
        SetIsSpawning(true);
        GetWorld()->SpawnActor<APIRangedEnemy>(m_ranged_add, this->GetActorLocation(), GetPlayerDirection(), spawn_params);
    }
}

AProjectInfernoPlayerCharacter* APITeleporter::GetPlayer()
{
    return Cast<AProjectInfernoPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

FRotator APITeleporter::GetPlayerDirection()
{
    return (GetPlayer()->GetActorLocation() - this->GetActorLocation()).Rotation();
}

bool APITeleporter::GetIsSpawning()
{
    return m_is_spawning;
}

void APITeleporter::SetIsSpawning(bool spawning)
{
    m_is_spawning = spawning;
}
