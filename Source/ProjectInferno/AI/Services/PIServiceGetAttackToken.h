// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "CoreMinimal.h"
#include "PIServiceGetAttackToken.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPIServiceGetAttackToken : public UBTService_BlackboardBase
{
    GENERATED_BODY()
public:
    virtual void TickNode(UBehaviorTreeComponent& owner, uint8* memory, float delta_time) override;
};
