// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "CoreMinimal.h"
#include "PICharacter.h"
#include "ProjectInfernoPatrolEnemyControl.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API AProjectInfernoPatrolEnemyControl : public AAIController
{
    GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class AProjectInfernoProjectile> m_projectile;

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
    float m_seconds_between_fire = 3.0f;

    // Set the radius of when the pawn will be detected
    UPROPERTY(EditAnywhere, Category = "AI")
    float m_detection_radius = 6400.0f;

    // Set the length of time before the detected pawn is removed from memory
    UPROPERTY(VisibleAnywhere, Category = "AI")
    float m_detection_age = 3.0f;

    // Set the radius of when the pawn will no longer be detected
    UPROPERTY(EditAnywhere, Category = "AI")
    float m_undetect_radius; // = m_detection_radius + 50.0f;

    // Set the vision field of view in degrees
    UPROPERTY(VisibleAnywhere, Category = "AI")
    float m_peripheral_vision_angle = 130.0f;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    class UAISenseConfig_Sight* m_sight_config;

    bool m_player_detected = false;

    float m_distance_to_player = 0.0f;

    float m_time_to_fire = 3.0f;

    class AActor* m_target;

public:
    AProjectInfernoPatrolEnemyControl();

    virtual void Tick(float delta_time) override;

    virtual FRotator GetControlRotation() const override;

    UFUNCTION()
    void PawnDetected(const TArray<AActor*>& pawns);

    UFUNCTION()
    void Fire();
};
