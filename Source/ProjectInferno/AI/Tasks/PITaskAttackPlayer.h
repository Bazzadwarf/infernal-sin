// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimMontage.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "TimerManager.h"
#include "PITaskAttackPlayer.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPITaskAttackPlayer : public UBTTask_BlackboardBase
{
    GENERATED_BODY()
private:
    UPROPERTY()
    UBehaviorTreeComponent* m_owner = nullptr;

    UPROPERTY()
    UAnimMontage* m_current_attack_animation = nullptr;

    FTimerDelegate m_attack_finished_delegate;
    FTimerHandle m_attack_timer_handle;

public:
    UPITaskAttackPlayer();

    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner, uint8* memory) override;

    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& owner, uint8* memory) override;

    void OnAttackTimerFinished();

private:
    void Cleanup(UBehaviorTreeComponent& owner);
};
