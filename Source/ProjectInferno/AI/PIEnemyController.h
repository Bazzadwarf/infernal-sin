// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ProjectInfernoPlayerCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "CoreMinimal.h"
#include "PIEnemy.h"
#include "PITokenManager.h"
#include "PIEnemyController.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API APIEnemyController : public AAIController
{
    GENERATED_BODY()
private:
    UPROPERTY(EditDefaultsOnly, Category = "AI", Meta = (AllowPrivateAccess = "true", DisplayName = "Blackboard"))
    UBlackboardComponent* m_blackboard;

    UPROPERTY(EditDefaultsOnly, Category = "AI", Meta = (AllowPrivateAccess = "true", DisplayName = "Behaviour Tree"))
    UBehaviorTreeComponent* m_behaviour_tree;

    UPROPERTY(EditDefaultsOnly,
              Category = "Movement",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Max Walking Speed"))
    float m_max_walking_speed = 500.0f;

    UPROPERTY(EditDefaultsOnly,
              Category = "Movement",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Max Running Speed"))
    float m_max_running_speed = 600.0f;

    UPROPERTY(EditDefaultsOnly,
              Category = "Animations",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Knockback Animation"))
    UAnimMontage* m_knockback_animation = nullptr;

    UPROPERTY()
    AProjectInfernoPlayerCharacter* m_player_reference = nullptr;

    UPROPERTY(EditDefaultsOnly,
              Category = "AI",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Attack Token Type"))
    EPIEnemyAttackTokenType m_attack_token_type;

    bool m_has_attack_token = false;

    bool m_is_running = false;
    bool m_is_attacking = false;
    bool m_is_stunned = false;

public:
    APIEnemyController();

    virtual void Possess(APawn* pawn) override;

    virtual void Tick(float delta_time) override;

    UFUNCTION()
    void OnPerceptionUpdated(const TArray<AActor*>& actors);

    UFUNCTION(BlueprintCallable)
    virtual void StartAttack();

    UFUNCTION(BlueprintCallable)
    virtual void StopAttack();

    UFUNCTION(BlueprintCallable)
    bool IsAttacking();

    UFUNCTION(BlueprintCallable)
    bool TryAcquireAttackToken();

    UFUNCTION(BlueprintCallable)
    bool HasAttackToken();

    UFUNCTION(BlueprintCallable)
    APIEnemy* GetEnemy();

    UFUNCTION(BlueprintCallable)
    AProjectInfernoPlayerCharacter* GetPlayerReference();

    UFUNCTION(BlueprintCallable)
    void SetIsRunning(bool is_running);

    UFUNCTION(BlueprintCallable)
    bool IsRunning();

    UFUNCTION(BlueprintCallable)
    void StartStun();

    UFUNCTION(BlueprintCallable)
    void EndStun();

    UFUNCTION(BlueprintCallable)
    bool IsStunned();

    UFUNCTION()
    void OnDamaged(FPIDamageInfo info);

    UFUNCTION()
    void OnDeath(FPIDamageInfo info);
};
