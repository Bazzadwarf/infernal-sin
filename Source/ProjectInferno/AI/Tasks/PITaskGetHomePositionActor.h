// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"
#include "PITaskGetHomePositionActor.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPITaskGetHomePositionActor : public UBTTask_BlackboardBase
{
    GENERATED_BODY()
public:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner, uint8* memory) override;
};
