// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PIBossController.h"
#include "PIExecutionerController.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API APIExecutionerController : public APIBossController
{
    GENERATED_BODY()
private:
    FTimerHandle m_timer_handle;

public:
    void Tick(float delta_time) override;

    BossPhases GetCurrentPhase() override;

    void Idle(float delta_time) override;

    void Phase1(float delta_time) override;

    void Phase2(float delta_time) override;

    void Phase3(float delta_time) override;

    void Phase4(float delta_time) override;

    void Phase1Melee();

    void Phase2Melee();

    void Phase3Melee();
};
