// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PICharacter.h"
#include "ProjectInfernoPlayerCharacter.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "PIPlayerController.generated.h"

UENUM(BlueprintType)
enum class EPIPlayerAttack : uint8
{
    LightMelee,
    HeavyMelee,
    LightProjectile,
    HeavyProjectile
};

UCLASS()
class PROJECTINFERNO_API APIPlayerController : public APlayerController
{
    GENERATED_BODY()
private:
    const float MAX_SPEED_DEFAULT = 600.0f;
    const float MAX_SPEED_SPRINTING = 800.0f;
    const float DODGE_ROLL_DISTANCE = 800.0f;
    const float DODGE_ROLL_TIME = 0.792f;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Meta = (AllowPrivateAccess = "true", DisplayName = "View Pitch (Maximum"))
    float m_view_pitch_max = 50.f;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Meta = (AllowPrivateAccess = "true", DisplayName = "View Pitch (Minimum)"))
    float m_view_pitch_min = -30.f;

    UPROPERTY(EditDefaultsOnly,
              Category = "Projectiles",
              meta = (AllowPrivateAccess = "true", DisplayName = "Heavy Projectile"))
    TSubclassOf<class AProjectInfernoProjectile> m_heavy_projectile;

    UPROPERTY(EditDefaultsOnly,
              Category = "Projectiles",
              meta = (AllowPrivateAccess = "true", DisplayName = "Light Projectile"))
    TSubclassOf<class AProjectInfernoProjectile> m_light_projectile;

    UPROPERTY(EditDefaultsOnly, Category = "UI", Meta = (BlueprintProtected = "true", DisplayName = "Pause Menu"))
    TSubclassOf<class UUserWidget> m_pause_menu_class;

    UPROPERTY()
    class UUserWidget* m_pause_menu_widget;

    UPROPERTY(EditDefaultsOnly, Category = "UI", Meta = (BlueprintProtected = "true", DisplayName = "Player HUD"))
    TSubclassOf<class UUserWidget> m_player_ui_class;

    UPROPERTY()
    class UUserWidget* m_player_ui_widget;

    UPROPERTY(EditDefaultsOnly,
              Category = "Animations|Attacks|Melee",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Light Melee Attack Animations"))
    TArray<UAnimMontage*> m_light_attack_animations;

    UPROPERTY(EditDefaultsOnly,
              Category = "Animations|Attacks|Melee",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Heavy Melee Attack Animations"))
    TArray<UAnimMontage*> m_heavy_attack_animations;

    UPROPERTY(EditDefaultsOnly,
              Category = "Animations|Attacks|Projectile",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Light Projectile Attack Animations"))
    TArray<UAnimMontage*> m_light_projectile_attack_animations;

    UPROPERTY(EditDefaultsOnly,
              Category = "Animations|Attacks|Projectile",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Heavy Projectile Attack Animation"))
    UAnimMontage* m_heavy_projectile_attack_animation;

    UPROPERTY(EditDefaultsOnly,
              Category = "Animations",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Knockback Animation"))
    UAnimMontage* m_knockback_animation;

    bool m_charmed = false;

    UPROPERTY()
    TArray<AActor*> m_hit_actors;

    FTimerHandle m_charm_timer_handle;

    // Movement Variables
    float m_movement_input_x = 0.0f;
    float m_movement_input_y = 0.0f;
    float m_dodge_distance_per_second = 0.0f;

    bool m_sprint_key_down = false;
    bool m_dodging = false;

    // Used in animation notifies to allow animation cancelling
    bool m_accept_movement_input = true;

    // Attack Variables
    bool m_can_increase_attack_chain = false;
    int m_attack_chain_count = 0;
    EPIPlayerAttack m_last_attack;

    // Used to prevent the player from attacking when playing the damaged animation
    bool m_accept_attack_input = true;

    // Used to disable damage when in the damaged animation
    float m_invulnerability_time_left = 0.0f;

    class UParticleSystemComponent* m_hit_particle_system_component;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Sounds",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Hurt Sounds"))
    TArray<USoundBase*> m_hurt_sounds;

    UPROPERTY(EditAnywhere,
        BlueprintReadWrite,
        Category = "Sounds",
        Meta = (AllowPrivateAccess = "true", DisplayName = "Sword Hit"))
    USoundBase* m_sword_hit;

public:
    virtual void PostInitializeComponents() override;

    virtual void Tick(float delta_time) override;

    virtual void OnPossess(APawn* pawn) override;

    virtual void OnUnPossess() override;

    void MoveForward(float value);

    void MoveSideways(float value);

    void StartSprinting();

    void StopSprinting();

    void StartDodge();

    void StopDodge();

    // Attack Methods
    void PerformLightAttack();

    void PerformHeavyAttack();

    void PerformLightProjectileAttack();

    void PerformHeavyProjectileAttack();

    void PerformAttack(EPIPlayerAttack attack);

    void FireProjectile(EPIPlayerAttack attack_type, FVector spawn_location);

    // Attack Chain Methods
    void ResetAttackChain();

    void SetAttackChainCanIncrease(bool can_increase);

    void Charm();

    virtual void Pause() override;

    UFUNCTION(BlueprintCallable, Category = "UI")
    void Resume();

    void TurnAtRate(float value);

    void LookUpAtRate(float value);

    UFUNCTION()
    void OnWeaponHit(UPrimitiveComponent* hit_component,
                     AActor* other_actor,
                     UPrimitiveComponent* other_component,
                     int32 other_body_index,
                     bool b_from_sweep,
                     const FHitResult& sweep_result);

    UFUNCTION()
    void OnPlayerDamaged(FPIDamageInfo info);

    UFUNCTION()
    void StopKnockbackAnimation();

    UFUNCTION(BlueprintCallable)
    float GetMovementInputX();

    UFUNCTION(BlueprintCallable)
    float GetMovementInputY();

    UFUNCTION(BlueprintCallable)
    bool IsSprinting();

    UFUNCTION(BlueprintCallable)
    bool IsDodging();

    UFUNCTION(BlueprintCallable)
    bool IsFalling();

    UFUNCTION(BlueprintCallable)
    void SetMovementInputEnabled(bool enabled);

    
protected:
    virtual void BeginPlay() override;

    virtual void SetupInputComponent() override;

private:
    class AProjectInfernoPlayerCharacter* GetPlayerCharacter();

    class UCharacterMovementComponent* GetCharacterMovement();

    void ApplyMovementToAxis(EAxis::Type axis, float value);

    void StopCurrentAttackChain();
};
