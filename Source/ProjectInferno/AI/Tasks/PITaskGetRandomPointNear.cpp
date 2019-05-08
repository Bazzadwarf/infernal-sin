// Fill out your copyright notice in the Description page of Project Settings.

#include "PITaskGetRandomPointNear.h"
#include "../../ProjectInfernoPlayerCharacter.h"
#include "../PIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UPITaskGetRandomPointNear::ExecuteTask(UBehaviorTreeComponent& owner, uint8* memory)
{
    auto enemy_controller = Cast<APIEnemyController>(owner.GetAIOwner());
    auto player = Cast<AActor>(owner.GetBlackboardComponent()->GetValueAsObject(m_player_key.SelectedKeyName));

    if (player)
    {
        auto angle = FMath::FRandRange(-180.0f, 180.0f);
        auto distance = FMath::FRandRange(m_min_distance, m_max_distance);

        auto new_loc = player->GetActorLocation();
        new_loc.X += distance * FMath::Cos(angle);
        new_loc.Y += distance * FMath::Sin(angle);

        owner.GetBlackboardComponent()->SetValueAsVector(m_point_vector_key.SelectedKeyName, new_loc);

        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
