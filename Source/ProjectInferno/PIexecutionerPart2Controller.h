// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "PIBossEnums.h"
#include "PITeleporter.h"
#include "Perception/AIPerceptionComponent.h"
#include "ProjectInfernoBoss.h"
#include "ProjectInfernoPlayerCharacter.h"
#include "ProjectInfernoProjectile.h"
#include "PIExecutionerPart2Controller.generated.h"

UCLASS()
class PROJECTINFERNO_API APIExecutionerPart2Controller : public AAIController
{
    GENERATED_BODY()

public:
    UBoxComponent* m_falling_box;

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

    int m_current_teleporter = 0;
    int m_previous_teleporter = 0;

    TArray<UActorComponent*> m_actor_comp_array;

    TArray<AActor*> m_hit_actors;

    BossPhases m_current_phase;
    BossStates m_current_state;
    BossStates m_previous_state;

    PlayerSide m_player_side = PlayerSide::Front;
    bool m_player_at_back = false;

    PlayerSide m_teleporter_side = PlayerSide::Front;

    float m_add_count_check_timer = 0;

    int m_add_counter = 0;

    int m_state_counter = 0;

    bool m_attacking = false;

    AProjectInfernoPlayerCharacter* m_player;

    bool m_player_detected = false;

    float m_attack_delay = 0;

    float m_stay_close_timer = 0;

    float m_player_far_timer = 0;

    FTimerHandle m_fire_delay_handle;

    float m_ranged_yaw = 0;
    float m_ranged_yaw_2 = 0;

    float m_ranged_timer = 0;

    int m_ranged_counter = 0;

    FVector m_ranged_spawn_pos = FVector{0, 0, 0};

    bool m_ranged = false;

public:
    APIExecutionerPart2Controller();

    virtual void Tick(float delta_time) override;

    virtual FRotator GetControlRotation() const override;

    AProjectInfernoBoss* GetBoss();

    virtual void OnBossDeath();

    UFUNCTION(BlueprintCallable)
    virtual void Kill();

    BossPhases GetCurrentPhase();

    virtual void SetCurrentPhase();

    virtual void Idle(float delta_time);

    virtual void Phase1(float delta_time);

    virtual void Phase2(float delta_time);

    virtual void Phase3(float delta_time);

    AProjectInfernoPlayerCharacter* GetPlayer();

    FRotator GetPlayerDirection();

    FVector GetPlayerLocation();

    float GetPlayerDistance();

    void ApproachPlayer(float acceptance);

    void StopFollowing();

    void ApproachLocation(FVector location, float acceptance);

    float GetTeleporterDistance();

    FRotator GetTeleporterDirection();

    FVector GetTeleporterLocation();

    UFUNCTION(BlueprintCallable)
    void FacePlayer();

    void FacePosition(FVector position_to_look);

    FVector GetPosition();

    void SetPosition(FVector position);

    UFUNCTION(BlueprintCallable)
    void Unfocus();

    UFUNCTION(BlueprintCallable)
    void Lunge2();

    UFUNCTION(BlueprintCallable)
    void StartFlying();

    UFUNCTION(BlueprintCallable)
    void StopFlying();

    UFUNCTION(BlueprintCallable)
    void SetIdle();

    UFUNCTION(BlueprintCallable)
    void EnableRootMotionRotation();

    UFUNCTION(BlueprintCallable)
    void DisableRootMotionRotation();

    UFUNCTION(BlueprintCallable)
    void EnableBodyCollider();

    UFUNCTION(BlueprintCallable)
    void DisableBodyCollider();

    UFUNCTION(BlueprintCallable)
    void SetRanged();

    UFUNCTION(BlueprintCallable)
    void HomingProjectiles();

    UFUNCTION(BlueprintCallable)
    void SpawnerProjectiles();

    UFUNCTION(BlueprintCallable)
    void AOEProjectiles(int projectiles);

    UFUNCTION(BlueprintCallable)
    void WaveAOEProjectiles(int projectiles, float delta_time);

    UFUNCTION(BlueprintCallable)
    void RandAOEProjectiles(int projectiles, float delta_time);

    UFUNCTION(BlueprintCallable)
    void SwipeLeftToRight(float delta_time);

    UFUNCTION(BlueprintCallable)
    void SwipeRightToLeft(float delta_time);

    UFUNCTION(BlueprintCallable)
    void ConeProjectiles(float delta_time);

    UFUNCTION(BlueprintCallable)
    void SpiralProjectiles(float delta_time);

    UFUNCTION(BlueprintCallable)
    void FrontalBarrage(float delta_time);

    void Fire(FVector location, FRotator rotation, TSubclassOf<class AProjectInfernoProjectile> projectile_type);

    UFUNCTION(BlueprintCallable)
    BossStates GetCurrentState();

    UFUNCTION()
    virtual void OnWeaponHit(UPrimitiveComponent* hit_component,
                             AActor* other_actor,
                             UPrimitiveComponent* other_component,
                             int32 other_body_index,
                             bool b_from_sweep,
                             const FHitResult& sweep_result);

    UFUNCTION()
    virtual void OnFallingHit(UPrimitiveComponent* hit_component,
                              AActor* other_actor,
                              UPrimitiveComponent* other_component,
                              int32 other_body_index,
                              bool b_from_sweep,
                              const FHitResult& sweep_result);

    void SetPlayerSide();

    bool RotateTowardsPlayer();

    void SetTeleporterSide();

    bool RotateTowardsTeleporter();

    void ClearHitActors();

protected:
    virtual void BeginPlay() override;
};
