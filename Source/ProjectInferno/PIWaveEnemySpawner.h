// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PIWaveEnemySpawner.generated.h"

UCLASS()
class PROJECTINFERNO_API APIWaveEnemySpawner : public AActor
{
    GENERATED_BODY()
public:
    // TODO CHANGE THIS TO NEW AI
    UFUNCTION(BlueprintCallable, Category = "Spawner")
    void SpawnEnemy(TSubclassOf<AProjectInfernoPatrolEnemy> EnemyType, APIWaveController* parent);

    UFUNCTION(BlueprintCallable, Category = "Spawner")
    void DestroySelf();
};
