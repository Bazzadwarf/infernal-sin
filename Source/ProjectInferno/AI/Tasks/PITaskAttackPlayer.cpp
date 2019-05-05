// Fill out your copyright notice in the Description page of Project Settings.

#include "PITaskAttackPlayer.h"
#include "../PIEnemyController.h"

EBTNodeResult::Type UPITaskAttackPlayer::ExecuteTask(UBehaviorTreeComponent& owner, uint8* memory)
{
    if (auto enemy_controller = Cast<APIEnemyController>(owner.GetAIOwner()))
    {
        enemy_controller->StartAttack();
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
