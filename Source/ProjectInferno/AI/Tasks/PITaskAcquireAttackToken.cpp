// Fill out your copyright notice in the Description page of Project Settings.

#include "PITaskAcquireAttackToken.h"
#include "../PIEnemyController.h"

EBTNodeResult::Type UPITaskAcquireAttackToken::ExecuteTask(UBehaviorTreeComponent& owner, uint8* memory)
{
    auto enemy_controller = Cast<APIEnemyController>(owner.GetAIOwner());

    if (enemy_controller->TryAcquireAttackToken())
    {
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
