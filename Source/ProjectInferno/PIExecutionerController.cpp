// Fill out your copyright notice in the Description page of Project Settings.

#include "PIExecutionerController.h"
#include "ConstructorHelpers.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectInfernoPlayerCharacter.h"

void APIExecutionerController::BeginPlay()
{
    Super::BeginPlay();

    m_current_phase = BossPhases::Phase_1;
}

void APIExecutionerController::Tick(float delta_time)
{
    Super::Tick(delta_time);

    randomPick = rand() % 6;

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

            /*case BossPhases::Phase_3:
                 Phase3(delta_time);
                 break;

             case BossPhases::Phase_4:
                 Phase4(delta_time);
                 break;*/
    }
}

void APIExecutionerController::SetCurrentPhase()
{
    if (GetBoss()->GetHealthPercent() > 0.6f || GetBoss()->GetHealthPercent() == 1.f)
    {
        // m_current_phase = BossPhases::Phase_4;
        m_current_phase = BossPhases::Phase_1;
    }
    else if (GetBoss()->GetHealthPercent() > 0.2f)
    {
        m_current_phase = BossPhases::Phase_2;
    }
    else if (GetBoss()->GetHealthPercent() < 0.0f)
    {
        // m_current_phase = BossPhases::Phase_2;
    }

    /*else if (GetBoss()->GetHealthPercent() > 0.1f)
    {
        m_current_phase = BossPhases::Phase_3;
    }
    else
    {
        m_current_phase = BossPhases::Phase_4;
    }*/
}

void APIExecutionerController::Idle(float delta_time)
{
    m_current_state = BossStates::Idle;
}

void APIExecutionerController::Phase1(float delta_time)
{
    if (RotateTowardsPlayer())
    {
        if (GetPlayerDistance() > 2000)
        {
            if (m_current_state == BossStates::Following || m_current_state == BossStates::Idle)
            {
                ApproachPlayer(100);
            }
        }
        else if (GetPlayerDistance() < 500)
        {
            if (m_current_state == BossStates::Following || m_current_state == BossStates::Idle)
            {
                Phase1Melee();
            }
        }
        else
        {
            if (m_current_state == BossStates::Following || m_current_state == BossStates::Idle)
            {
                m_current_state = BossStates::Melee5;
            }
        }
    }
}

void APIExecutionerController::Phase1Melee()
{
    ClearHitActors();
    StopMovement();

    if (m_previous_attack == BossStates::Melee1)
    {
        SetFocus(nullptr);

        m_current_state = BossStates::Idle;
        m_current_state = BossStates::Melee5;
        m_previous_attack = BossStates::Melee5;
    }
    else
    {
        SetFocus(nullptr);

        m_current_state = BossStates::Idle;
        m_current_state = BossStates::Melee1;
        m_previous_attack = BossStates::Melee1;
    }
}

void APIExecutionerController::Phase1Ranged()
{
    if (GetPlayerDistance() >= 500)
    {
        if (m_previous_attack == BossStates::ConeProjectiles)
        {
            FrontalBarage();
            m_previous_attack = BossStates::SwipeLeftToRight;
        }
        if (m_previous_attack == BossStates::SwipeRightToLeft)
        {
            ConeProjectiles();
            m_previous_attack = BossStates::ConeProjectiles;
        }
        else if (m_previous_attack == BossStates::SwipeLeftToRight)
        {
            SwipeRightToLeft();
            m_previous_attack = BossStates::SwipeRightToLeft;
        }
        else
        {
            SwipeLeftToRight();
            m_previous_attack = BossStates::SwipeLeftToRight;
        }
    }
}

void APIExecutionerController::Phase2(float delta_time)
{
    if (RotateTowardsPlayer())
    {
        if (GetPlayerDistance() > 2000)
        {
            if (m_current_state == BossStates::Following || m_current_state == BossStates::Idle)
            {
                ApproachPlayer(100);
            }
        }
        else if (GetPlayerDistance() < 500)
        {
            if (m_current_state == BossStates::Following || m_current_state == BossStates::Idle)
            {
                Phase1Melee();
            }
        }
        else
        {
            if (m_current_state == BossStates::Following || m_current_state == BossStates::Idle)
            {
                if (m_previous_attack == BossStates::SwipeRightToLeft)
                {
                    m_current_state = BossStates::SwipeRightToLeft;
                }
                else if (m_previous_attack == BossStates::SwipeLeftToRight)
                {
                    m_current_state = BossStates::LeftAttack;
                }
                else if (m_previous_attack == BossStates::AOEProjectiles)
                {
                    m_current_state = BossStates::StompAttack;
                }
                else if (m_previous_attack == BossStates::ConeProjectiles)
                {
                    m_current_state = BossStates::Melee5;
                }
                else if (m_previous_attack == BossStates::WaveAOEProjectiles)
                {
                    m_current_state = BossStates::AxeStompAttack;
                }
                else if (m_previous_attack == BossStates::Attacking)
                {
                    m_current_state = BossStates::Melee5;
                }
                else
                {
                    m_previous_attack = random[randomPick];
                }
            }
        }
    }
}
void APIExecutionerController::Phase2Melee()
{
    ClearHitActors();
    StopMovement();

    if (FMath::RandRange(1, 2) == 1)
    {
        if (m_previous_attack == BossStates::Melee4)
        {
            return;
        }

        SetFocus(nullptr);

        m_current_state = BossStates::Idle;
        m_current_state = BossStates::Melee4;
        m_previous_attack = BossStates::Melee4;
    }
    else
    {
        if (m_previous_attack == BossStates::Melee2)
        {
            return;
        }

        SetFocus(nullptr);

        m_current_state = BossStates::Idle;
        m_current_state = BossStates::Melee2;
        m_previous_attack = BossStates::Melee2;
    }
}

void APIExecutionerController::Phase2Ranged()
{
    if (GetPlayerDistance() >= 500)
    {
        if (m_previous_attack == BossStates::SwipeRightToLeft)
        {
            SwipeLeftToRight();
            m_previous_attack = random[randomPick];
        }
        else if (m_previous_attack == BossStates::WaveAOEProjectiles)
        {
            WaveAOEProjectiles(m_num_wave);
            m_previous_attack = random[randomPick];
        }
        else if (m_previous_attack == BossStates::AOEProjectiles)
        {
            AOEProjectiles(m_num_aoe);
            m_previous_attack = random[randomPick];
        }
        else if (m_previous_attack == BossStates::ConeProjectiles)
        {
            ConeProjectiles();
            m_previous_attack = random[randomPick];
        }
        else if (m_previous_attack == BossStates::SwipeLeftToRight)
        {
            SwipeLeftToRight();
            m_previous_attack = random[randomPick];
        }
        else if (m_previous_attack == BossStates::Attacking)
        {
            FrontalBarage();
            m_previous_attack = random[randomPick];
        }
        else
        {
            m_previous_attack = random[randomPick];
        }
    }
}

void APIExecutionerController::Phase3(float delta_time)
{ /*
    if (RotateTowardsPlayer())
    {
        if (GetPlayerDistance() > 1000.0f)
        {
            ApproachPlayer(0);

            // This should be switched out for projectiles with Sine waves attached to them
            if (!GetWorldTimerManager().TimerExists(m_timer_handle))
             {
                 auto attack_callback = [&, p = 16]() mutable { AOEProjectiles(p); };

                 GetWorldTimerManager().SetTimer(m_timer_handle, attack_callback, 1.0f, true);
             }
        }
        else
        {
            if (GetWorldTimerManager().TimerExists(m_timer_handle))
            {
                GetWorldTimerManager().ClearTimer(m_timer_handle);
            }

            int rand = FMath::RandRange(0, 100);

            if (rand < 75)
            {
                Phase3Melee();
            }
            else if (rand >= 75 && rand <= 100)
            {
                RandAOEProjectiels(60);
            }
        }
    }*/
}

void APIExecutionerController::Phase3Melee()
{
    /* ClearHitActors();
     StopMovement();
     SetFocus(nullptr);

     if (GetPlayerDistance() < 100.f)
     {
         m_current_state = BossStates::Idle;
         m_current_state = BossStates::StompAttack;
         m_previous_attack = BossStates::StompAttack;
         return;
     }

     int rand = FMath::RandRange(1, 4);
     SetFocus(nullptr);
     m_current_state = BossStates::Idle;

     switch (rand)
     {
         Sweep Attack case 1 : m_current_state = BossStates::Melee5;
         m_previous_attack = BossStates::Melee5;
         break;
         Ram Attack case 2 : m_current_state = BossStates::Melee2;
         m_previous_attack = BossStates::Melee2;
         break;
         Left Attack case 3 : m_current_state = BossStates::LeftAttack;
         m_previous_attack = BossStates::LeftAttack;
         break;
         Right Attack case 4 : m_current_state = BossStates::RightAttack;
         m_previous_attack = BossStates::RightAttack;
         break;
         default:
             m_current_state = BossStates::StompAttack;
             m_previous_attack = BossStates::StompAttack;
             break;
     }*/
}

void APIExecutionerController::Phase3Ranged()
{
}

void APIExecutionerController::Phase4(float delta_time)
{ /*
    if (RotateTowardsPlayer())
    {
        if (GetPlayerDistance() > 1000.0f)
        {
            ApproachPlayer(0);

             //This should be switched out for projectiles with Sine waves attached to them
            if (!GetWorldTimerManager().TimerExists(m_timer_handle))
             {
                 auto attack_callback = [&, p = 16]() mutable { AOEProjectiles(p); };

                 GetWorldTimerManager().SetTimer(m_timer_handle, attack_callback, 1.0f, true);
             }
        }
        else
        {
            if (GetWorldTimerManager().TimerExists(m_timer_handle))
            {
                GetWorldTimerManager().ClearTimer(m_timer_handle);
            }

            Phase3Melee();
            RandAOEProjectiels(60);
        }
    }

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
    }*/
}

void APIExecutionerController::Phase4Melee()
{
}

void APIExecutionerController::Phase4Ranged()
{
}
