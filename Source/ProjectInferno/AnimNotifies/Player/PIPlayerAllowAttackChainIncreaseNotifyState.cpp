// Fill out your copyright notice in the Description page of Project Settings.

#include "PIPlayerAllowAttackChainIncreaseNotifyState.h"
#include "PIPlayerController.h"
#include "ProjectInfernoPlayerCharacter.h"

void UPIPlayerAllowAttackChainIncreaseNotifyState::NotifyBegin(USkeletalMeshComponent* mesh_component,
                                                               UAnimSequenceBase* animation,
                                                               float total_duration)
{
    auto player = Cast<AProjectInfernoPlayerCharacter>(mesh_component->GetOwner());

    if (player)
    {
        auto controller = Cast<APIPlayerController>(player->GetController());

        if (controller)
        {
            controller->SetAttackChainCanIncrease(true);
        }
    }
}

void UPIPlayerAllowAttackChainIncreaseNotifyState::NotifyTick(USkeletalMeshComponent* mesh_component,
                                                              UAnimSequenceBase* animation,
                                                              float frame_delta_time)
{
}

void UPIPlayerAllowAttackChainIncreaseNotifyState::NotifyEnd(USkeletalMeshComponent* mesh_component,
                                                             UAnimSequenceBase* animation)
{
    auto player = Cast<AProjectInfernoPlayerCharacter>(mesh_component->GetOwner());

    if (player)
    {
        auto controller = Cast<APIPlayerController>(player->GetController());

        if (controller)
        {
            controller->SetAttackChainCanIncrease(false);
        }
    }
}
