// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "PICharacter.h"
#include "PIHeavyAdd.generated.h"

UCLASS()
class PROJECTINFERNO_API APIHeavyAdd : public APICharacter
{
    GENERATED_BODY()

private:
    UPROPERTY(VisibleDefaultsOnly,
              Category = "Weapon",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Left Hand Collider"))
    UBoxComponent* m_left_hand_collider = nullptr;

    UPROPERTY(VisibleDefaultsOnly,
              Category = "Weapon",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Right Hand Collider"))
    UBoxComponent* m_right_hand_collider = nullptr;

    UPROPERTY(VisibleDefaultsOnly,
              Category = "Weapon",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Body Collider"))
    UBoxComponent* m_body_collider = nullptr;

    bool m_dead = false;

    float m_dead_timer = 0;

public:
    APIHeavyAdd();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float delta_time) override;

    void EnableLeftHandColliders();

    void DisableLeftHandColliders();

    void EnableRightHandColliders();

    void DisableRightHandColliders();

    void EnableBodyCollider();

    void DisableBodyCollider();

    UBoxComponent* GetLeftHandCollider();

    UBoxComponent* GetRightHandCollider();

    UBoxComponent* GetBodyCollider();

    UFUNCTION()
    float GetHealth();

    UFUNCTION(BlueprintPure, Category = "Health")
    FText GetHealthText();

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealthPercent();

    UFUNCTION()
    void RecoverHealth(float healing);

    UFUNCTION()
    virtual void OnDeath(FPIDamageInfo info);

    void Dead();

    UFUNCTION()
    virtual void OnDamaged(FPIDamageInfo info);
};
