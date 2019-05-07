// Fill out your copyright notice in the Description page of Project Settings.

#include "PIEnemyRanged.h"
#include "PIEnemyController.h"

void APIEnemyRanged::BeginPlay()
{
    Super::BeginPlay();

    if (auto weapon = GetWeapon())
    {
        weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");
    }
}

void APIEnemyRanged::FireProjectileAtPlayer(bool is_weapon_socket, FName spawn_socket_name, FVector velocity)
{
    // TODO:
    USkeletalMeshComponent* skeletal_mesh = nullptr;

    if (is_weapon_socket)
    {
        if (auto weapon = GetWeapon())
        {
            skeletal_mesh = weapon->GetMesh();
        }
        else
        {
            return;
        }
    }
    else
    {
        skeletal_mesh = GetMesh();
    }

    auto socket_location = skeletal_mesh->GetSocketLocation(spawn_socket_name);
    auto direction = (UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation() - socket_location).Rotation();

    FActorSpawnParameters spawn_params;
    spawn_params.Owner = GetController();
    spawn_params.Instigator = this;

    auto projectile = GetWorld()->SpawnActor<AProjectInfernoProjectile>(
        m_projectile_type, socket_location, direction, spawn_params);

    if (projectile)
    {
        projectile->FireInDirection(direction.Vector());
    }
}
