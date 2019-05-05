// Fill out your copyright notice in the Description page of Project Settings.

#include "PIPlayerSpawnProjectileAtSocketNotify.h"
#include "PIPlayerController.h"
#include "ProjectInfernoPlayerCharacter.h"

void UPIPlayerSpawnProjectileAtSocketNotify::Notify(USkeletalMeshComponent* mesh_component,
                                                    UAnimSequenceBase* animation)
{
    auto player = Cast<AProjectInfernoPlayerCharacter>(mesh_component->GetOwner());

    if (player)
    {
        auto weapon_mesh = Cast<USkeletalMeshComponent>(player->GetWeapon()->GetMesh());
        auto controller = Cast<APIPlayerController>(player->GetController());

        if (controller)
        {
            switch (m_projectile_type)
            {
                case EPIPlayerAttack::LightProjectile:
                {
                    auto socket_location = mesh_component->GetSocketLocation(m_socket_name);
                    controller->FireProjectile(m_projectile_type, socket_location);
                }
                break;
                case EPIPlayerAttack::HeavyProjectile:
                {
                    auto socket_location = weapon_mesh->GetSocketLocation(m_socket_name);
                    controller->FireProjectile(m_projectile_type, socket_location);
                }
                break;
            }
        }
    }
}
