// Fill out your copyright notice in the Description page of Project Settings.

#include "PICharacterSetMovementModeNotify.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPICharacterSetMovementModeNotify::Notify(USkeletalMeshComponent* mesh_component,
                                                        UAnimSequenceBase* animation)
{
    auto character = Cast<APICharacter>(mesh_component->GetOwner());

    if (character != nullptr)
    {
        character->GetCharacterMovement()->SetMovementMode(m_movement_mode);
    }
}
