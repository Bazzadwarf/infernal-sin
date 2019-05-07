// Fill out your copyright notice in the Description page of Project Settings.

#include "PIEnemyEndAttackNotify.h"
#include "AI/PIEnemy.h"
#include "AI/PIEnemyController.h"

void UPIEnemyEndAttackNotify::Notify(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation)
{
    auto enemy = Cast<APIEnemy>(mesh_component->GetOwner());

    if (enemy)
    {
        auto controller = Cast<APIEnemyController>(enemy->GetController());

        if (controller)
        {
            controller->StopAttack();
        }
    }
}
