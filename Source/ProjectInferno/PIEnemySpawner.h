// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PIEnemySpawner.generated.h"

UCLASS()
class PROJECTINFERNO_API APIEnemySpawner : public AActor
{
    GENERATED_BODY()
private:
    UPROPERTY(EditAnywhere, Category = Spawner, meta = (AllowPrivateAccess = "true"))
    TSubclassOf<class AProjectInfernoPatrolEnemy> m_enemy;

    float m_spawn_timer = 5.0f;
    float m_death_timer = 20.0f;

public:
    // Sets default values for this actor's properties
    APIEnemySpawner();

    // Called every frame
    virtual void Tick(float delta_time) override;

    UFUNCTION()
    void SpawnEnemy();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
