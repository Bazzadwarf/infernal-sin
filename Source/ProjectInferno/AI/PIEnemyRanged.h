// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../ProjectInfernoProjectile.h"
#include "AI/PIEnemy.h"
#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "PIEnemyRanged.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API APIEnemyRanged : public APIEnemy
{
    GENERATED_BODY()
private:
    UPROPERTY(EditDefaultsOnly,
              Category = "Weapon",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Projectile Type"))
    TSubclassOf<AProjectInfernoProjectile> m_projectile_type;

public:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void FireProjectileAtPlayer(bool is_weapon_socket, FName spawn_socket_name, FVector velocity);
};
