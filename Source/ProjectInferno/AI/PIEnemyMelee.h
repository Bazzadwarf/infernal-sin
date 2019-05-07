// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/PIEnemy.h"
#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "PIEnemyMelee.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API APIEnemyMelee : public APIEnemy
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

    UPROPERTY(EditDefaultsOnly,
              Category = "Weapon",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Attack Damage"))
    float m_attack_damage = 10.0f;

public:
    APIEnemyMelee();

    virtual void BeginPlay() override;

    void EnableHandColliders();

    void DisableHandColliders();

    UBoxComponent* GetLeftHandCollider();

    UBoxComponent* GetRightHandCollider();

    float GetAttackDamage();
};
