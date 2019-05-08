// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "ProjectInfernoPlayerCharacter.h"
#include "ProjectInfernoProjectile.generated.h"

UCLASS()
class PROJECTINFERNO_API AProjectInfernoProjectile : public AActor
{
    GENERATED_BODY()
private:
    // Movement
    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Projectile,
              meta = (AllowPrivateAccess = "true", DisplayName = "Starting Speed"))
    float m_starting_speed = 10.0f;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Projectile,
              meta = (AllowPrivateAccess = "true", DisplayName = "Starting Acceleration"))
    FVector m_acceleration = FVector::ZeroVector;

    FVector m_internal_velocity = FVector::ZeroVector;

    FVector m_internal_direction = FVector::ZeroVector;

    FVector m_internal_location = FVector::ZeroVector;

    FVector m_world_location = FVector::ZeroVector;

    // Sine Abilities
    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Sine_Abilities,
              meta = (AllowPrivateAccess = "true", DisplayName = "Sine Wave"))
    bool m_sine_wave = false;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Sine_Abilities,
              meta = (AllowPrivateAccess = "true", DisplayName = "Sine Wave Freq"))
    float m_sine_wave_per_life = 1.0f;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Projectile,
              meta = (AllowPrivateAccess = "true", DisplayName = "Lifetime Duration"))
    float m_initial_life_span = 5.0f;

    float m_life_time = 0.0f;

    float m_sine_wave_length = 0.0f;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Sine_Abilities,
              meta = (AllowPrivateAccess = "true", DisplayName = "Sine Horizonal"))
    bool m_sine_horizontal = false;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Sine_Abilities,
              meta = (AllowPrivateAccess = "true", DisplayName = "Sine Verticle"))
    bool m_sine_verticle = false;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Sine_Abilities,
              meta = (AllowPrivateAccess = "true", DisplayName = "Sine Depth"))
    bool m_sine_depth = false;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Sine_Abilities,
              meta = (AllowPrivateAccess = "true", DisplayName = "Sine Magnitude"))
    float m_sine_magnitude = 100.0f;

    float sine;
	
	// Cos Abilities
    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Cos_Abilities,
              meta = (AllowPrivateAccess = "true", DisplayName = "Cos Wave"))
    bool m_cos_wave = false;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Cos_Abilities,
              meta = (AllowPrivateAccess = "true", DisplayName = "Cos Wave Freq"))
    float m_cos_wave_per_life = 1.0f;

    float m_cos_wave_length = 0.0f;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Cos_Abilities,
              meta = (AllowPrivateAccess = "true", DisplayName = "Cos Horizonal"))
    bool m_cos_horizontal = false;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Cos_Abilities,
              meta = (AllowPrivateAccess = "true", DisplayName = "Cos Verticle"))
    bool m_cos_verticle = false;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Cos_Abilities,
              meta = (AllowPrivateAccess = "true", DisplayName = "Cos Depth"))
    bool m_cos_depth = false;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = Cos_Abilities,
              meta = (AllowPrivateAccess = "true", DisplayName = "Cos Magnitude"))
    float m_cos_magnitude = 100.0f;

    float cos;


    // Collision component
    UPROPERTY(VisibleDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
    USphereComponent* m_collision_component;

    UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class APIEnemySpawner> m_portal;

    UPROPERTY(VisibleAnywhere, Category = Movement, meta = (AllowPrivateAccess = "true"))
    UProjectileMovementComponent* m_projectile_movement_component;

    UPROPERTY(EditDefaultsOnly, Category = Projectile, meta = (AllowPrivateAccess = "true"))
    float m_damage = 10;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Projectile, meta = (AllowPrivateAccess = "true"))
    float m_homing_speed = 500.0f;

    class AProjectInfernoPlayerCharacter* m_player;

    class AActor* m_target;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Particle Systems",
              Meta = (AllowPrivateAccess = "true", DisplayName = "On Destroy"))
    class UParticleSystem* m_on_destory_particle_effect;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Sounds",
              Meta = (AllowPrivateAccess = "true", DisplayName = "On Destroy"))
    class USoundBase* m_on_destroy_sound;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Sounds",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Attenuation"))
    class USoundAttenuation* m_attenuation;

public:
    // Sets default values for this actor's properties
    AProjectInfernoProjectile();

    // Called every frame
    virtual void Tick(float delta_time) override;

    void FireInDirection(const FVector& shoot_direction);

    UFUNCTION()
    void OnHit(UPrimitiveComponent* hit_component,
               AActor* other_actor,
               UPrimitiveComponent* other_component,
               FVector normal_impulse,
               const FHitResult& hit);

    UFUNCTION()
    void FindPlayer();

    UFUNCTION()
    float GetHomingSpeed();

    UFUNCTION()
    void SetHomingSpeed(float speed);

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
