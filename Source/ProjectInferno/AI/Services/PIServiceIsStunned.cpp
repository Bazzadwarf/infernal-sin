// Fill out your copyright notice in the Description page of Project Settings.

#include "PIServiceIsStunned.h"
#include "../PIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UPIServiceIsStunned::TickNode(UBehaviorTreeComponent& owner, uint8* memory, float delta_time)
{
    if (auto enemy_controller = Cast<APIEnemyController>(owner.GetAIOwner()))
    {
        if (auto stunned = enemy_controller->IsStunned())
        {
            owner.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), true);
        }
        else
        {
            owner.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        }
    }
}
