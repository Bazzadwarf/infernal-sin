// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "PICharacter.h"
#include "ProjectInfernoBasicEnemyController.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API AProjectInfernoBasicEnemyController : public AAIController
{
    GENERATED_BODY()

private:
    /* Blackboard key */
    UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    FName m_blackboard_target_key = "Target";

    /* Blackboard key */
    UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    FName m_blackboard_chase_target_key = "ChaseTarget";

    /* Behavior Tree component reference */
    UBehaviorTreeComponent* m_behaviour_tree_component;

    /* Blackboard comp ref */
    UBlackboardComponent* m_blackboard_component;

public:
    /* Constructor */
    AProjectInfernoBasicEnemyController();

    /* Executes right when the controller possess a Pawn */
    virtual void OnPossess(APawn* pawn) override;

    /* Sets the sensed target in the blackboard */
    void SetSeenTarget(APawn* pawn);
};
