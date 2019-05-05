// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/TriggerBox.h"
#include "PIGameInstance.h"
#include "PIWaveEnemySpawner.h"
#include "ProjectInfernoPatrolEnemy.h"
#include "PIWaveController.generated.h"

// Contains the info for each enemy type in a wave
USTRUCT(BlueprintType)
struct FPIEnemyStruct
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Info")
    TSubclassOf<AProjectInfernoPatrolEnemy> enemy_type;

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Enemy Info")
    int num_to_spawn;
};

// Contains the info for an wave
USTRUCT(BlueprintType)
struct FPIWaveStruct
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Wave Info")
    TArray<FPIEnemyStruct> enemies;
};

USTRUCT(BlueprintType)
struct FPISpawnerStruct
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditANywhere, Category = "Spawner Info")
    class APIWaveEnemySpawner* enemySpawner;
        
    int timersPicked = 0;
};

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API APIWaveController : public ATriggerBox
{
    GENERATED_BODY()
private:
    // The waves
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TArray<FPIWaveStruct> m_waves;

    int m_wave_index = 0;

    int m_num_enemies_left = -1;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Spawners",
              meta = (AllowPrivateAccess = "true", DisplayName = "Wave Spawners"))
    TArray<FPISpawnerStruct> m_spawners;

    APIWaveEnemySpawner* m_last_selection;

public:
    APIWaveController();

    virtual void Tick(float delta_time) override;

    virtual void NotifyActorBeginOverlap(AActor* actor) override;

    void SpawnNextWave();

    void EnemyDied();

protected:
    virtual void BeginPlay() override;
};
