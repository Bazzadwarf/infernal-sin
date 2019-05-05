// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectInfernoBasicEnemyController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ProjectInfernoBasicEnemy.h"

AProjectInfernoBasicEnemyController::AProjectInfernoBasicEnemyController()
{
    m_behaviour_tree_component = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviourComp"));
    m_blackboard_component = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
}

void AProjectInfernoBasicEnemyController::Possess(APawn* pawn)
{
    Super::Possess(pawn);

    // Check if possessed pawn is of type AProjectInfernoBasicEnemy
    if (auto ai_character = Cast<AProjectInfernoBasicEnemy>(pawn))
    {
        // Initialise Blackboard and BehaviourTree
        if (ai_character->GetBehaviorTree()->BlackboardAsset)
        {
            m_blackboard_component->InitializeBlackboard(*(ai_character->GetBehaviorTree()->BlackboardAsset));
            m_behaviour_tree_component->StartTree(*(ai_character->GetBehaviorTree()));
        }
    }
}

void AProjectInfernoBasicEnemyController::SetSeenTarget(APawn* pawn)
{
    // Registers the Pawn that the AI has seen in the blackboard
    if (m_blackboard_component)
    {
        if (pawn != NULL)
        {
            m_blackboard_component->SetValueAsObject(m_blackboard_target_key, pawn);
            m_blackboard_component->SetValueAsBool(m_blackboard_chase_target_key, true);
        }
        else
        {
            m_blackboard_component->ClearValue(m_blackboard_target_key);
            m_blackboard_component->SetValueAsBool(m_blackboard_chase_target_key, false);
        }
    }
}
