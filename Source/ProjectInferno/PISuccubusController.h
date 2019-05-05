// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PIBossController.h"
#include "PISuccubusController.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API APISuccubusController : public APIBossController
{
    GENERATED_BODY()

public:
    void Tick(float delta_time) override;

    BossPhases GetCurrentPhase() override;

    void Idle(float delta_time) override;

    void Phase1(float delta_time) override;

    void Phase2(float delta_time) override;

    void Phase3(float delta_time) override;

protected:
    void BeginPlay() override;
};
