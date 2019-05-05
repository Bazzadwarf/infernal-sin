// Fill out your copyright notice in the Description page of Project Settings.

#include "PISuccubusController.h"

void APISuccubusController::BeginPlay()
{
    Super::BeginPlay();

    CharmProjectiles();
}

void APISuccubusController::Tick(float delta_time)
{
    Super::Tick(delta_time);

    if (FMath::Abs(GetPlayerDirection().Yaw - GetControlRotation().Yaw) <= 5)
    {
        switch (m_current_phase)
        {
            case BossPhases::Phase_1:
                Phase1(delta_time);
                break;

            case BossPhases::Phase_2:
                Phase2(delta_time);
                break;

            case BossPhases::Phase_3:
                Phase3(delta_time);
                break;

            default:
                Idle(delta_time);
        }
    }
    else if (m_current_state == BossStates::Idle || m_current_state == BossStates::Following)
    {
        m_current_state = BossStates::Following;

        if (GetPlayerDirection().Yaw < 0 && GetControlRotation().Yaw < 0)
        {
            if (GetPlayerDirection().Yaw < GetControlRotation().Yaw)
            {
                GetBoss()->AddActorLocalRotation(FRotator(0, -4, 0));
            }
            else
            {
                GetBoss()->AddActorLocalRotation(FRotator(0, 4, 0));
            }
        }
        else if (GetPlayerDirection().Yaw > 0 && GetControlRotation().Yaw > 0)
        {
            if (GetPlayerDirection().Yaw < GetControlRotation().Yaw)
            {
                GetBoss()->AddActorLocalRotation(FRotator(0, -4, 0));
            }
            else
            {
                GetBoss()->AddActorLocalRotation(FRotator(0, 4, 0));
            }
        }
        else
        {
            if (GetPlayerDirection().Yaw > 0 && GetControlRotation().Yaw < 0)
            {
                if ((GetPlayerDirection().Yaw + FMath::Abs(GetControlRotation().Yaw)) < 180)
                {
                    GetBoss()->AddActorLocalRotation(FRotator(0, 4, 0));
                }
                else
                {
                    GetBoss()->AddActorLocalRotation(FRotator(0, -4, 0));
                }
            }
            else
            {
                if ((GetControlRotation().Yaw + FMath::Abs(GetPlayerDirection().Yaw)) < 180)
                {
                    GetBoss()->AddActorLocalRotation(FRotator(0, -4, 0));
                }
                else
                {
                    GetBoss()->AddActorLocalRotation(FRotator(0, 4, 0));
                }
            }
        }
    }
}

BossPhases APISuccubusController::GetCurrentPhase()
{
    if (GetBoss()->GetHealthPercent() > 0.75f)
    {
        return BossPhases::Phase_1;
    }

    if (GetBoss()->GetHealthPercent() > 0.35f)
    {
        return BossPhases::Phase_2;
    }

    return BossPhases::Phase_3;
}

void APISuccubusController::Idle(float delta_time)
{
    m_current_state = BossStates::Idle;
}

void APISuccubusController::Phase1(float delta_time)
{
    FacePlayer();
}

void APISuccubusController::Phase2(float delta_time)
{
    SetFocus(nullptr);
    ApproachPlayer(100);
}

void APISuccubusController::Phase3(float delta_time)
{
    StopMovement();
    SpiralProjectiles();
}
