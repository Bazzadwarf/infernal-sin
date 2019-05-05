// Fill out your copyright notice in the Description page of Project Settings.

#include "PIExecutionerController.h"
#include "ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectInfernoPlayerCharacter.h"

void APIExecutionerController::Tick(float delta_time)
{
    Super::Tick(delta_time);

    switch (m_current_phase)
    {
        case BossPhases::Idle_Phase:
            Idle(delta_time);
            break;

        case BossPhases::Phase_1:
            Phase1(delta_time);
            break;

        case BossPhases::Phase_2:
            Phase2(delta_time);
            break;

        case BossPhases::Phase_3:
            Phase3(delta_time);
            break;

        case BossPhases::Phase_4:
            Phase4(delta_time);
            break;
    }
}

BossPhases APIExecutionerController::GetCurrentPhase()
{
    if (GetBoss()->GetHealthPercent() > 0.7f)
    {
        return BossPhases::Phase_1;
    }

    if (GetBoss()->GetHealthPercent() > 0.4f)
    {
        return BossPhases::Phase_2;
    }

    if (GetBoss()->GetHealthPercent() > 0.1f)
    {
        return BossPhases::Phase_3;
    }

    return BossPhases::Phase_4;
}

void APIExecutionerController::Idle(float delta_time)
{
    m_current_state = BossStates::Idle;
}

void APIExecutionerController::Phase1(float delta_time)
{
    if (RotateTowardsPlayer())
    {
        if (GetPlayerDistance() > 400)
        {
            if (m_current_state == BossStates::Following || m_current_state == BossStates::Idle)
            {
                ApproachPlayer(100);
            }
        }
        else
        {
            if (m_current_state == BossStates::Following || m_current_state == BossStates::Idle)
            {
                Phase1Melee();
            }
        }
    }
}

void APIExecutionerController::Phase2(float delta_time)
{
    if (RotateTowardsPlayer())
    {
        StopMovement();
        SetFocus(nullptr);

        if (FMath::RandRange(1, 2) != 1)
        {
            if (m_current_state == BossStates::Idle || m_current_state == BossStates::Following)
            {
                m_current_state = BossStates::AxeStompAttack;
                m_previous_attack = BossStates::AxeStompAttack;
            }
        }
        else
        {
            if (GetPlayerDistance() > 400)
            {
                if (m_current_state == BossStates::Following || m_current_state == BossStates::Idle)
                {
                    ApproachPlayer(100);
                }
            }
            else
            {
                if (m_current_state == BossStates::Following || m_current_state == BossStates::Idle)
                {
                    Phase2Melee();
                }
            }
        }
    }
}

void APIExecutionerController::Phase3(float delta_time)
{
    if (RotateTowardsPlayer())
    {
        if (FMath::RandRange(1, 3) != 1)
        {
            if (GetPlayerDistance() > 400)
            {
                if (m_current_state == BossStates::Following || m_current_state == BossStates::Idle)
                {
                    ApproachPlayer(100);
                }
            }
            else
            {
                if (m_current_state == BossStates::Following || m_current_state == BossStates::Idle)
                {
                    Phase3Melee();
                }
            }
        }
        else
        {
            if (m_current_state == BossStates::Idle)
            {
                StopMovement();
                SetFocus(nullptr);

                if (m_previous_attack == BossStates::AxeStompAttack)
                {
                    return;
                }

                m_current_state = BossStates::AxeStompAttack;
                m_previous_attack = BossStates::AxeStompAttack;
            }
        }
    }
    else
    {
        if (m_previous_attack != BossStates::RightAttack && m_previous_attack != BossStates::LeftAttack
            && GetPlayerDistance() < 400)
        {
            if (m_player_side == PlayerSide::Right)
            {
                StopMovement();
                SetFocus(nullptr);

                m_current_state = BossStates::Idle;
                m_current_state = BossStates::RightAttack;
                m_previous_attack = BossStates::RightAttack;
            }
            else if (m_player_side == PlayerSide::Left)
            {
                StopMovement();
                SetFocus(nullptr);

                m_current_state = BossStates::Idle;
                m_current_state = BossStates::LeftAttack;
                m_previous_attack = BossStates::LeftAttack;
            }
        }
    }
}

void APIExecutionerController::Phase4(float delta_time)
{
    if (RotateTowardsPlayer())
    {
        if (m_current_state == BossStates::Idle)
        {
            StopMovement();
            SetFocus(nullptr);

            if (GetPlayerDistance() < 800)
            {
                if (FMath::RandRange(1, 2) == 1)
                {
                    if (m_previous_attack == BossStates::StompAttack)
                    {
                        return;
                    }

                    m_current_state = BossStates::StompAttack;
                    m_previous_attack = BossStates::StompAttack;
                }
                else
                {
                    if (m_previous_attack == BossStates::AxeStompAttack)
                    {
                        return;
                    }

                    m_current_state = BossStates::AxeStompAttack;
                    m_previous_attack = BossStates::AxeStompAttack;
                }
            }
            else
            {
                m_current_state = BossStates::AxeStompAttack;
                m_previous_attack = BossStates::AxeStompAttack;
            }
        }
    }
}

void APIExecutionerController::Phase1Melee()
{
    ClearHitActors();
    StopMovement();

    if (FMath::RandRange(1, 2) == 1)
    {
        if (m_previous_attack == BossStates::RamAttack)
        {
            return;
        }

        SetFocus(nullptr);

        m_current_state = BossStates::Idle;
        m_current_state = BossStates::RamAttack;
        m_previous_attack = BossStates::RamAttack;
    }
    else
    {
        if (m_previous_attack == BossStates::SweepAttack)
        {
            return;
        }

        SetFocus(nullptr);

        m_current_state = BossStates::Idle;
        m_current_state = BossStates::SweepAttack;
        m_previous_attack = BossStates::SweepAttack;
    }
}

void APIExecutionerController::Phase2Melee()
{
    ClearHitActors();
    StopMovement();
    SetFocus(nullptr);

    m_current_state = BossStates::Idle;
    m_current_state = BossStates::SweepAttack;
    m_previous_attack = BossStates::SweepAttack;
}

void APIExecutionerController::Phase3Melee()
{
    ClearHitActors();
    StopMovement();

    if (FMath::RandRange(1, 2) == 1)
    {
        if (m_previous_attack == BossStates::RamAttack)
        {
            return;
        }

        SetFocus(nullptr);

        m_current_state = BossStates::Idle;
        m_current_state = BossStates::RamAttack;
        m_previous_attack = BossStates::RamAttack;
    }
    else
    {
        if (m_previous_attack == BossStates::OverheadAttack)
        {
            return;
        }

        SetFocus(nullptr);

        m_current_state = BossStates::Idle;
        m_current_state = BossStates::OverheadAttack;
        m_previous_attack = BossStates::OverheadAttack;
    }
}
