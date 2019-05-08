// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PIWaveEnemySpawner.generated.h"

UCLASS()
class PROJECTINFERNO_API APIWaveEnemySpawner : public AActor
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Particle Systems",
              Meta = (AllowPrivateAccess = "true", DisplayName = "On Destory"))
    class UParticleSystem* m_on_spawn_particle_effect;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Sounds",
              Meta = (AllowPrivateAccess = "true", DisplayName = "On Destroy"))
    class USoundBase* m_on_spawn_sound;

public:
    // TODO CHANGE THIS TO NEW AI
    UFUNCTION(BlueprintCallable, Category = "Spawner")
    void SpawnEnemy(TSubclassOf<APIEnemy> enemy_type, APIWaveController* parent);

    UFUNCTION(BlueprintCallable, Category = "Spawner")
    void DestroySelf();
};
