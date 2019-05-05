// Fill out your copyright notice in the Description page of Project Settings.

#include "PICharacterToggleWeaponColliderNotifyState.h"

void UPICharacterToggleWeaponColliderNotifyState::NotifyBegin(USkeletalMeshComponent* mesh_component,
                                                              UAnimSequenceBase* animation,
                                                              float total_duration)
{
    auto character = Cast<APICharacter>(mesh_component->GetOwner());

    if (character != nullptr)
    {
        auto weapon = character->GetWeapon();

        if (weapon != nullptr)
        {
            weapon->EnableWeaponCollider();
        }
    }
}

void UPICharacterToggleWeaponColliderNotifyState::NotifyTick(USkeletalMeshComponent* mesh_component,
                                                             UAnimSequenceBase* animation,
                                                             float frame_delta_time)
{
}

void UPICharacterToggleWeaponColliderNotifyState::NotifyEnd(USkeletalMeshComponent* mesh_component,
                                                            UAnimSequenceBase* animation)
{
    auto character = Cast<APICharacter>(mesh_component->GetOwner());

    if (character != nullptr)
    {
        auto weapon = character->GetWeapon();

        if (weapon != nullptr)
        {
            weapon->DisableWeaponCollider();
        }
    }
}
