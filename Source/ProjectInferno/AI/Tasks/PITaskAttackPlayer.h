// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"
#include "PITaskAttackPlayer.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPITaskAttackPlayer : public UBTTask_BlackboardBase
{
    GENERATED_BODY()
public:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner, uint8* memory) override;
};
