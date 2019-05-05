// Fill out your copyright notice in the Description page of Project Settings.

#include "PIPlayerToggleMovementInputNotify.h"
#include "PIPlayerController.h"
#include "ProjectInfernoPlayerCharacter.h"

void UPIPlayerToggleMovementInputNotify::Notify(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation)
{
    auto player = Cast<AProjectInfernoPlayerCharacter>(mesh_component->GetOwner());

    if (player)
    {
        auto controller = Cast<APIPlayerController>(player->GetController());

        if (controller)
        {
            controller->SetMovementInputEnabled(m_movement_input_enabled);
        }
    }
}
