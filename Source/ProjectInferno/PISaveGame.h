// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PISaveGame.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPISaveGame : public USaveGame
{
    GENERATED_BODY()
public:
    UPROPERTY(VisibleAnywhere, Category = Basic)
    FString player_name = "Player Name";

    UPROPERTY(VisibleAnywhere, Category = Level)
    FString level_name = "Prison";

    UPROPERTY(VisibleAnywhere, Category = Level)
    FString checkpoint_actor_tag = "DefaultSpawnPoint";

    UPROPERTY(VisibleAnywhere, Category = Player)
    float player_health = 100.0f;

    UPROPERTY(VisibleAnywhere, Category = Player)
    int player_ammo_count = 3;

    UPROPERTY(VisibleAnywhere, Category = Player)
    float player_ammo_percentage = 0.0f;

    UFUNCTION(BlueprintCallable)
    void ResetPlayerValuesToDefault()
    {
        player_health = 100.0f;
        player_ammo_count = 3;
        player_ammo_percentage = 0.0f;
    }

    UFUNCTION(BlueprintCallable)
    void ResetCheckpointToDefault()
    {
        checkpoint_actor_tag = "DefaultSpawnPoint";
    }
};
