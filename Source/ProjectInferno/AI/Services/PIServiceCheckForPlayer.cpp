// Fill out your copyright notice in the Description page of Project Settings.

#include "PIServiceCheckForPlayer.h"
#include "../PIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UPIServiceCheckForPlayer::TickNode(UBehaviorTreeComponent& owner, uint8* memory, float delta_time)
{
    if (auto enemy_controller = Cast<APIEnemyController>(owner.GetAIOwner()))
    {
        if (auto player_reference = enemy_controller->GetPlayerReference())
        {
            owner.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), player_reference);
        }
        else
        {
            owner.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        }
    }
}
