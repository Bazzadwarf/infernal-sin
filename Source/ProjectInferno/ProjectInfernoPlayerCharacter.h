// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "Components/ChildActorComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Navigation/CrowdAgentInterface.h"
#include "PICharacter.h"
#include "Perception/AISightTargetInterface.h"
#include "ProjectInfernoPlayerCharacter.generated.h"

UCLASS()
class PROJECTINFERNO_API AProjectInfernoPlayerCharacter : public APICharacter,
                                                          public IAISightTargetInterface,
                                                          public ICrowdAgentInterface
{
    GENERATED_BODY()

private:
    const float MAX_STAMINA = 100.0f;
    const int MAX_PROJECTILE_COUNT = 3;

    UPROPERTY(VisibleDefaultsOnly)
    USpringArmComponent* m_camera_boom = nullptr;

    UPROPERTY(VisibleDefaultsOnly)
    UCameraComponent* m_camera_component = nullptr;

    int m_current_projectile_count = MAX_PROJECTILE_COUNT;

    float m_projectile_regen_progress = 0.0f;

    float m_current_stamina = MAX_STAMINA;

    UPROPERTY()
    class UUserWidget* m_death_screen_widget;

    UPROPERTY(EditDefaultsOnly, Category = "UI", Meta = (BlueprintProtected = "true", DisplayName = "Death Screen"))
    TSubclassOf<class UUserWidget> m_death_screen_class;

public:
    AProjectInfernoPlayerCharacter();

    virtual void Tick(float delta_time) override;

    virtual bool CanBeSeenFrom(const FVector& observer_location,
                               FVector& out_seen_location,
                               int32& number_of_los_checks_performed,
                               float& out_sight_strength,
                               const AActor* ignore_actor = nullptr) const;

    UFUNCTION()
    float GetHealth();

    UFUNCTION(BlueprintPure, Category = "Health")
    FText GetHealthText();

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealthPercent();

    UFUNCTION()
    float GetStamina();

    UFUNCTION(BlueprintPure, Category = "Stamina")
    FText GetStaminaText();

    UFUNCTION(BlueprintPure, Category = "Stamina")
    float GetStaminaPercent();

    UFUNCTION()
    void UseStamina(float stamina_use);

    UFUNCTION()
    void RecoverStamina(float stamina_recovery);

    UFUNCTION()
    int GetCurrentProjectileCount();

    UFUNCTION(BlueprintCallable)
    float GetProjectileRegenProgress(int projectile);

    UFUNCTION()
    void ConsumeProjectileReserves(int amount);

    UFUNCTION()
    void RegenerateProjectile(float amount);

    UFUNCTION()
    void SetCameraLag(float lag);

    UFUNCTION()
    void OnDeath();

    UFUNCTION()
    void Charm();

protected:
    virtual void BeginPlay() override;

private:
    bool CalculateHit(const FVector& observer_location,
                      FVector& out_seen_location,
                      int32& number_of_los_checks_performed,
                      float& out_sight_strength,
                      const AActor* ignore_actor,
                      FVector hit_location) const;
};
