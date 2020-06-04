// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "PIBossEnums.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "ProjectInfernoBoss.h"
#include "ProjectInfernoPlayerCharacter.h"
#include "ProjectInfernoProjectile.h"
#include "TimerManager.h"
#include "PIBossController.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API APIBossController : public AAIController
{
    GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly,
              Category = "Projectile",
              meta = (AllowPrivateAccess = "true", DisplayName = "Regular Projectile"))
    TSubclassOf<class AProjectInfernoProjectile> m_regular_projectile;

    UPROPERTY(EditDefaultsOnly,
              Category = "Projectile",
              meta = (AllowPrivateAccess = "true", DisplayName = "Spawner Projectile"))
    TSubclassOf<class AProjectInfernoProjectile> m_spawner_projectile;

    UPROPERTY(EditDefaultsOnly,
              Category = "Projectile",
              meta = (AllowPrivateAccess = "true", DisplayName = "Homing Projectile"))
    TSubclassOf<class AProjectInfernoProjectile> m_homing_projectile;

    UPROPERTY(EditDefaultsOnly,
              Category = "Projectile",
              meta = (AllowPrivateAccess = "true", DisplayName = "Charm Projectile"))
    TSubclassOf<class AProjectInfernoProjectile> m_charm_projectile;

    UPROPERTY(EditDefaultsOnly,
              Category = "Projectile",
              meta = (AllowPrivateAccess = "true", DisplayName = "Rapid Projectile"))
    TSubclassOf<class AProjectInfernoProjectile> m_rapid_projectile;

    UPROPERTY(EditDefaultsOnly,
              Category = "Projectile",
              meta = (AllowPrivateAccess = "true", DisplayName = "Slow Projectile"))
    TSubclassOf<class AProjectInfernoProjectile> m_slow_projectile;

    UPROPERTY(EditDefaultsOnly,
              Category = "Projectile",
              meta = (AllowPrivateAccess = "true", DisplayName = "Bounce Projectile"))
    TSubclassOf<class AProjectInfernoProjectile> m_bounce_projectile;

    FTimerHandle m_fire_delay_handle;

    TArray<AActor*> m_hit_actors;

protected:
    UPROPERTY(EditDefaultsOnly,
              Category = "Projectile",
              meta = (AllowPrivateAccess = "true", DisplayName = "Num of AOE Projectiles"))
    int m_num_aoe = 16;

    UPROPERTY(EditDefaultsOnly,
              Category = "Projectile",
              meta = (AllowPrivateAccess = "true", DisplayName = "Num of Wave Projectiles"))
    int m_num_wave = 16;

    BossPhases m_current_phase;
    BossStates m_current_state;
    BossStates m_previous_state;
    BossStates m_previous_attack;
    PlayerSide m_player_side = PlayerSide::Front;

    int m_state_counter = 0;

    bool m_attacking = false;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    bool m_player_detected = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    float m_distance_to_player = 3000.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    float m_detection_radius = 3000.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    float m_detection_age = 5.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    float m_undetect_radius = m_detection_radius + 50.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    float m_peripheral_vision_angle = 360.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
    class UAISenseConfig_Sight* m_sight_config;

public:
    APIBossController();

    virtual void Tick(float delta_time) override;

    virtual void OnPossess(APawn* pawn) override;

    virtual FRotator GetControlRotation() const override;

    UFUNCTION()
    void PawnDetected(const TArray<AActor*>& pawns);

    AProjectInfernoBoss* GetBoss();

    virtual void OnBossDeath();

    UFUNCTION(BlueprintCallable)
    virtual void Kill();

    BossPhases GetCurrentPhase();

    virtual void SetCurrentPhase();

    virtual void Idle(float delta_time);

    virtual void Phase1(float delta_time);

    virtual void Phase1Melee();

    virtual void Phase1Ranged();

    virtual void Phase2(float delta_time);

    virtual void Phase2Melee();

    virtual void Phase2Ranged();

    virtual void Phase3(float delta_time);

    virtual void Phase3Melee();

    virtual void Phase3Ranged();

    virtual void Phase4(float delta_time);

    virtual void Phase4Melee();

    virtual void Phase4Ranged();

    AProjectInfernoPlayerCharacter* GetPlayer();

    FRotator GetPlayerDirection();

    FVector GetPlayerLocation();

    float GetPlayerDistance();

    void ApproachPlayer(float acceptance);

    void ApproachLocation(FVector location, float acceptance);

    void FacePlayer();

    void FacePosition(FVector position_to_look);

    FVector GetPosition();

    void SetPosition(FVector position);

    UFUNCTION(BlueprintCallable)
    void IsAttacking(bool is_attacking);

    UFUNCTION(BlueprintCallable)
    void SetIdle();

    UFUNCTION(BlueprintCallable)
    void PerformRangedAttack();

    UFUNCTION(BlueprintCallable)
    void HomingProjectiles();

    UFUNCTION(BlueprintCallable)
    void CharmProjectiles();

    UFUNCTION(BlueprintCallable)
    void RapidProjectiles();

    UFUNCTION(BlueprintCallable)
    void SpawnerProjectiles();

    UFUNCTION(BlueprintCallable)
    void AOEProjectiles(int projectiles);

    UFUNCTION(BlueprintCallable)
    void WaveAOEProjectiles(int projectiles);

    UFUNCTION(BlueprintCallable)
    void RandAOEProjectiels(int projectiles);

    UFUNCTION(BlueprintCallable)
    void ConeProjectiles();

    UFUNCTION(BlueprintCallable)
    void ConeProjectilesReverse();

    UFUNCTION(BlueprintCallable)
    void SwipeLeftToRight();

    UFUNCTION(BlueprintCallable)
    void SwipeRightToLeft();

    UFUNCTION(BlueprintCallable)
    void SpiralProjectiles();

    UFUNCTION(BlueprintCallable)
    void FrontalBarage();

    void Fire(FVector location,
              FRotator rotation,
              FActorSpawnParameters spawn_params,
              TSubclassOf<class AProjectInfernoProjectile> projectile_type);

    UFUNCTION(BlueprintCallable)
    BossStates GetCurrentState();

    UFUNCTION()
    virtual void OnWeaponHit(UPrimitiveComponent* hit_component,
                             AActor* other_actor,
                             UPrimitiveComponent* other_component,
                             int32 other_body_index,
                             bool b_from_sweep,
                             const FHitResult& sweep_result);

    bool RotateTowardsPlayer();

    void ClearHitActors();

protected:
    virtual void BeginPlay() override;
};
