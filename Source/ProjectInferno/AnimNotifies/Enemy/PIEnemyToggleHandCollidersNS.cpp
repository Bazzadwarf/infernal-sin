// Fill out your copyright notice in the Description page of Project Settings.

#include "PIEnemyToggleHandCollidersNS.h"
#include "../../AI/PIEnemyMelee.h"
#include "../../AI/PIEnemyController.h"
#include "Components/SkeletalMeshComponent.h"

void UPIEnemyToggleHandCollidersNS::NotifyBegin(USkeletalMeshComponent* mesh_component,
                                                UAnimSequenceBase* animation,
                                                float total_duration)
{
    auto enemy = Cast<APIEnemyMelee>(mesh_component->GetOwner());

    if (enemy)
    {
        enemy->EnableHandColliders();
    }
}

void UPIEnemyToggleHandCollidersNS::NotifyTick(USkeletalMeshComponent* mesh_component,
                                               UAnimSequenceBase* animation,
                                               float frame_delta_time)
{
}

void UPIEnemyToggleHandCollidersNS::NotifyEnd(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation)
{
    auto enemy = Cast<APIEnemyMelee>(mesh_component->GetOwner());

    if (enemy)
    {
        enemy->DisableHandColliders();
    }
}
