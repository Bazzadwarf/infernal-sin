// Fill out your copyright notice in the Description page of Project Settings.

#include "PIEnemyEndStunNotify.h"
#include "../../AI/PIEnemyController.h"

void UPIEnemyEndStunNotify::Notify(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation)
{
    auto enemy = Cast<APIEnemy>(mesh_component->GetOwner());

    if (enemy)
    {
        auto controller = Cast<APIEnemyController>(enemy->GetController());

        if (controller)
        {
            controller->EndStun();
        }
    }
}
