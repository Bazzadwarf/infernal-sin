// Fill out your copyright notice in the Description page of Project Settings.

#include "PICharacterEquipWeaponNotify.h"

void UPICharacterEquipWeaponNotify::Notify(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation)
{
    auto character = Cast<APICharacter>(mesh_component->GetOwner());

    if (character != nullptr)
    {
        auto weapon = character->GetWeapon();

        if (weapon != nullptr)
        {
            weapon->AttachToComponent(
                mesh_component, FAttachmentTransformRules::SnapToTargetNotIncludingScale, m_socket_name);
        }
    }
}
