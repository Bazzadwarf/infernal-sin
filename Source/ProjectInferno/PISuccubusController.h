// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PIBossController.h"
#include "PITeleporter.h"
#include "PISuccubusController.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API APISuccubusController : public APIBossController
{
    GENERATED_BODY()

private:
    float m_timer = 0;
    bool m_started = false;

    float m_teleport_timer = 0;
    int m_current_teleporter = 0;
    int m_last_teleporter = 0;

public:
    void Tick(float delta_time) override;

    void SetCurrentPhase() override;

    void Idle(float delta_time) override;

    void Phase1(float delta_time) override;

    void Phase2(float delta_time) override;

    void Phase3(float delta_time) override;

    void Teleport(int teleport_num);

    void Phase1Melee();

    void Phase3Melee();

protected:
    void BeginPlay() override;
};
