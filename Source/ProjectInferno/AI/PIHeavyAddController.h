// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "PIHeavyAdd.h"
#include "CoreMinimal.h"
#include "ProjectInfernoPlayerCharacter.h"
#include "ProjectInfernoProjectile.h"
#include "PIHeavyAddController.generated.h"

UENUM(BlueprintType)
enum class HeavyStates : uint8
{
    Idle,
    Moving,
    Backswipe,
    Stomp,
    Swipe,
    Charge1,
    Charge2,
    Charge3,
    Knockback1,
    Knockback2,
    Knockback3,
    Dead
};

UENUM(BlueprintType)
enum class PlayersSide : uint8
{
    Front,
    Left,
    Right
};

UCLASS()
class PROJECTINFERNO_API APIHeavyAddController : public AAIController
{
    GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly,
              Category = "Projectiles",
              meta = (AllowPrivateAccess = "true", DisplayName = "Regular Projectile"))
    TSubclassOf<class AProjectInfernoProjectile> m_regular_projectile;

    TArray<AActor*> m_hit_actors;

    HeavyStates m_current_state;
    HeavyStates m_previous_state;

    AProjectInfernoPlayerCharacter* m_player;

    PlayersSide m_player_side = PlayersSide::Front;
    bool m_player_at_back = false;

    bool m_player_detected = false;

    float m_stay_close_timer = 0;

    float m_player_far_timer = 0;

    bool m_is_stunned = false;

    float m_cumulated_damage = 0;

    float m_damage_maintained_timer = 0;

    float m_ranged_timer = 0;

    int m_ranged_counter = 0;

    FVector m_ranged_spawn_pos = FVector{0, 0, 0};

    bool m_ranged = false;

public:
    APIHeavyAddController(const FObjectInitializer& object_initializer);

    virtual void BeginPlay() override;

    virtual void Tick(float delta_time) override;

    virtual FRotator GetControlRotation() const override;

    UFUNCTION(BlueprintCallable)
    HeavyStates GetCurrentState();

    virtual void Idle(float delta_time);

    virtual void Attack(float delta_time);

    FVector GetPosition();

    APIHeavyAdd* GetHeavyAdd();

    AProjectInfernoPlayerCharacter* GetPlayer();

    FRotator GetPlayerDirection();

    FVector GetPlayerLocation();

    float GetPlayerDistance();

    void ApproachPlayer(float acceptance);

    void StopFollowing();

    UFUNCTION(BlueprintCallable)
    virtual void StopStun();

    UFUNCTION(BlueprintCallable)
    virtual void Kill();

    UFUNCTION(BlueprintCallable)
    void FacePlayer();

    UFUNCTION(BlueprintCallable)
    void Unfocus();

    UFUNCTION(BlueprintCallable)
    void StartCharge();

    UFUNCTION(BlueprintCallable)
    void EndCharge();

    UFUNCTION(BlueprintCallable)
    void SetIdle();

    UFUNCTION(BlueprintCallable)
    void EnableRootMotionRotation();

    UFUNCTION(BlueprintCallable)
    void DisableRootMotionRotation();

    UFUNCTION(BlueprintCallable)
    void EnableRightHandCollider();

    UFUNCTION(BlueprintCallable)
    void EnableLeftHandCollider();

    UFUNCTION(BlueprintCallable)
    void EnableBodyCollider();

    UFUNCTION(BlueprintCallable)
    void DisableAllColliders();

    UFUNCTION(BlueprintCallable)
    void SetRanged();

    UFUNCTION(BlueprintCallable)
    void AOEProjectiles(int projectiles);

    UFUNCTION(BlueprintCallable)
    void WaveAOEProjectiles(int projectiles, float delta_time);

    UFUNCTION(BlueprintCallable)
    void RandAOEProjectiles(int projectiles, float delta_time);

    UFUNCTION(BlueprintCallable)
    void FrontalBarrage(float delta_time);

    void Fire(FVector location, FRotator rotation, TSubclassOf<class AProjectInfernoProjectile> projectile_type);

    void SetPlayerSide();

    bool RotateTowardsPlayer();

    void ClearHitActors();

    UFUNCTION()
    void OnDamaged(FPIDamageInfo info);

    UFUNCTION()
    void OnDeath(FPIDamageInfo info);

    UFUNCTION()
    void OnMeleeHit(UPrimitiveComponent* hit_component,
                    AActor* other_actor,
                    UPrimitiveComponent* other_component,
                    int32 other_body_index,
                    bool b_from_sweep,
                    const FHitResult& sweep_result);
};
