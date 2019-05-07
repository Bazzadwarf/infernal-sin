// Fill out your copyright notice in the Description page of Project Settings.

#include "PITaskGetHomePositionActor.h"
#include "../PIEnemyController.h"
#include "../PIEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UPITaskGetHomePositionActor::ExecuteTask(UBehaviorTreeComponent& owner, uint8* memory)
{
    if (auto enemy_controller = Cast<APIEnemyController>(owner.GetAIOwner()))
    {
        if (auto enemy = enemy_controller->GetEnemy())
        {
            if (auto home_position = enemy->GetHomePositionActor())
            {
                owner.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), home_position);
                return EBTNodeResult::Succeeded;
            }
        }
    }

    return EBTNodeResult::Failed;
}
