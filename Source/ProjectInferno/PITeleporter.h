// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PIEnemySpawner.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "PITeleporter.generated.h"

UCLASS()
class PROJECTINFERNO_API APITeleporter : public AActor
{
    GENERATED_BODY()

private:
    UPROPERTY(EditDefaultsOnly, Category = "Teleporter", meta = (AllowPrivateAccess = "true", DisplayName = "Spawner"))
    TSubclassOf<class APIEnemySpawner> m_spawner;

    UPROPERTY(EditAnywhere, Category = "Teleporter", meta = (AllowPrivateAccess = "true", DisplayName = "IsCenter"))
    bool m_is_center = false;

public:
    bool m_was_last = false;
    bool m_is_current = false;

public:
    APITeleporter();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void MakeSpawner();
};
