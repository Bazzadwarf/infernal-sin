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

void APITeleporter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (m_is_current)
    {
    }
}

void APITeleporter::MakeSpawner()
{
    FActorSpawnParameters spawn_params;
    spawn_params.Owner = this;
    GetWorld()->SpawnActor<APIEnemySpawner>(
        m_spawner, this->GetActorLocation(), this->GetActorRotation(), spawn_params);
}
