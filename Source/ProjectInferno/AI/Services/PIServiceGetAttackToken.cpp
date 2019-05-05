// Fill out your copyright notice in the Description page of Project Settings.

#include "PIServiceGetAttackToken.h"
#include "../PIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UPIServiceGetAttackToken::TickNode(UBehaviorTreeComponent& owner, uint8* memory, float delta_time)
{
    UE_LOG(LogTemp, Warning, TEXT("Ticking..."))
    if (auto enemy_controller = Cast<APIEnemyController>(owner.GetAIOwner()))
    {
        if (auto has_token = enemy_controller->TryAcquireAttackToken())
        {
            owner.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), has_token);
        }
        else
        {
            owner.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
        }
    }
}
