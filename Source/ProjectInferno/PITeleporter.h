// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/PIEnemyMelee.h"
#include "AI/PIEnemyRanged.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectInfernoPlayerCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "PITeleporter.generated.h"

UCLASS()
class PROJECTINFERNO_API APITeleporter : public AActor
{
    GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly, Category = "Teleporter", meta = (AllowPrivateAccess = "true", DisplayName = "Spawner"))
    TSubclassOf<class APIEnemySpawner> m_spawner;

    UPROPERTY(EditDefaultsOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true", DisplayName = "Light Add"))
    TSubclassOf<class APIEnemyMelee> m_light_add;

    UPROPERTY(EditDefaultsOnly, Category = "Spawner", meta = (AllowPrivateAccess = "true", DisplayName = "Ranged Add"))
    TSubclassOf<class APIEnemyRanged> m_ranged_add;

    UPROPERTY(EditAnywhere, Category = "Teleporter", meta = (AllowPrivateAccess = "true", DisplayName = "IsCenter"))
    bool m_is_center = false;

    float m_particle_timer = 0;

public:
    bool m_was_last = false;
    bool m_is_current = false;
    bool m_is_spawning = false;

public:
    APITeleporter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float delta_time) override;

    void SpawnLightAdd();

    void SpawnRangedAdd();

    UFUNCTION(BlueprintCallable)
    bool GetIsSpawning();
    UFUNCTION(BlueprintCallable)
    void SetIsSpawning(bool spawning);

    AProjectInfernoPlayerCharacter* GetPlayer();

    FRotator GetPlayerDirection();
};
