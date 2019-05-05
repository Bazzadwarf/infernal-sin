// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ProjectInfernoGameMode.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API AProjectInfernoGameMode : public AGameMode
{
    GENERATED_BODY()
public:
    AProjectInfernoGameMode();

    virtual AActor* ChoosePlayerStart_Implementation(AController* player) override;
};
