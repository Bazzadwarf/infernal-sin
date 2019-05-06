// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "CoreMinimal.h"
#include "PITaskGetRandomPointNear.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPITaskGetRandomPointNear : public UBTTaskNode
{
    GENERATED_BODY()
private:
    UPROPERTY(EditAnywhere, Category = "Blackboard", Meta = (AllowPrivateAccess = "true", DisplayName = "Player Key"))
    FBlackboardKeySelector m_player_key;

    UPROPERTY(EditAnywhere,
              Category = "Blackboard",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Point Vector Key"))
    FBlackboardKeySelector m_point_vector_key;

    UPROPERTY(EditAnywhere, Category = "Options", Meta = (AllowPrivateAccess = "true", DisplayName = "Minimum Distance From Player"))
    float m_min_distance = 180.0f;

    UPROPERTY(EditAnywhere, Category = "Options", Meta = (AllowPrivateAccess = "true", DisplayName = "Maximum Distance From Player"))
    float m_max_distance = 220.0f;

public:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner, uint8* memory) override;
};
