// Fill out your copyright notice in the Description page of Project Settings.

#include "PITaskAttackPlayer.h"
#include "../PIEnemyController.h"

UPITaskAttackPlayer::UPITaskAttackPlayer()
{
    bCreateNodeInstance = true;

    m_attack_finished_delegate = FTimerDelegate::CreateUObject(this, &UPITaskAttackPlayer::OnAttackTimerFinished);
}

EBTNodeResult::Type UPITaskAttackPlayer::ExecuteTask(UBehaviorTreeComponent& owner, uint8* memory)
{
    m_owner = &owner;

    auto enemy_controller = Cast<APIEnemyController>(owner.GetAIOwner());

    // Reset timer handle
    m_attack_timer_handle.Invalidate();

    // Get random attack animation
    auto& all_attack_animations = enemy_controller->GetAttackAnimationsArray();

    if (all_attack_animations.Num() == 0)
    {
        return EBTNodeResult::Failed;
    }

    auto random_index = FMath::RandRange(0, all_attack_animations.Num() - 1);
    auto animation = all_attack_animations[random_index];

    // Play the animation and set a timer for the length of the animation
    const auto animation_length = enemy_controller->GetEnemy()->PlayAnimMontage(animation);
    enemy_controller->GetWorldTimerManager().SetTimer(
        m_attack_timer_handle, m_attack_finished_delegate, animation_length, false);

    return EBTNodeResult::InProgress;
}

// On task aborted: stop the animation
EBTNodeResult::Type UPITaskAttackPlayer::AbortTask(UBehaviorTreeComponent& owner, uint8* memory)
{
    auto enemy_controller = Cast<APIEnemyController>(owner.GetAIOwner());

    if (enemy_controller && m_attack_timer_handle.IsValid())
    {
        enemy_controller->GetWorldTimerManager().ClearTimer(m_attack_timer_handle);
    }

    m_attack_timer_handle.Invalidate();

    if (m_current_attack_animation)
    {
        enemy_controller->GetEnemy()->StopAnimMontage(m_current_attack_animation);
    }

    Cleanup(owner);

    return EBTNodeResult::Aborted;
}

// On timer finished, complete the task
void UPITaskAttackPlayer::OnAttackTimerFinished()
{
    if (m_owner)
    {
        Cleanup(*m_owner);
        FinishLatentTask(*m_owner, EBTNodeResult::Succeeded);
    }
}

void UPITaskAttackPlayer::Cleanup(UBehaviorTreeComponent& owner)
{
    // Stop the attack in case of task abortion or completion
    auto enemy_controller = Cast<APIEnemyController>(m_owner->GetAIOwner());
    enemy_controller->StopAttack();

    m_current_attack_animation = nullptr;
}
