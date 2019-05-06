// Fill out your copyright notice in the Description page of Project Settings.

#include "PISuccubusController.h"

void APISuccubusController::BeginPlay()
{
    Super::BeginPlay();

    m_current_phase = BossPhases::Phase_1;
}

void APISuccubusController::Tick(float delta_time)
{
    Super::Tick(delta_time);

    if (RotateTowardsPlayer())
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
}

void APISuccubusController::SetCurrentPhase()
{
    if (GetBoss()->GetHealthPercent() > 0.75f)
    {
        m_current_phase = BossPhases::Phase_1;
    }

    if (GetBoss()->GetHealthPercent() > 0.3f)
    {
        m_current_phase = BossPhases::Phase_2;
    }

    m_current_phase = BossPhases::Phase_3;
}

void APISuccubusController::Idle(float delta_time)
{
    m_current_state = BossStates::Idle;
}

void APISuccubusController::Phase1(float delta_time)
{
    FacePlayer();
    if (GetPlayerDistance() <= 600.f && GetBoss()->m_teleporters.Num() != 0)
    {
        m_teleport_timer += delta_time;
        if (m_teleport_timer >= 6)
        {
            SetFocus(nullptr);
            StopMovement();
            m_teleport_timer = 0;
            Teleport(FMath::RandRange(0, GetBoss()->m_teleporters.Num() - 1));
        }
        else
        {
            if (GetPlayerDistance() <= 100)
            {
                Phase1Melee();
            }
            else
            {
                ApproachPlayer(50);
            }
        }
    }
    else
    {
        m_teleport_timer = 0;
    }
}

void APISuccubusController::Phase2(float delta_time)
{
    // Stop looking at the player
    // SetFocus(nullptr);

    if (m_started == false)
    {
        m_current_state = BossStates::TakingOff;
        m_started = true;
    }

    if (m_current_state == BossStates::TakingOff)
    {
        UE_LOG(LogTemp, Warning, TEXT("I'm taking off"));

        // Fly up
        FVector original_position = GetPosition();
        float new_z = original_position.Z + 600;
        SetPosition({original_position.X, original_position.Y, new_z});
        GetBoss()->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

        // Set the timer for the next state
        m_timer = 30;

        // Change the state
        m_current_state = BossStates::Flying;
    }
    else if (m_current_state == BossStates::Flying)
    {
        UE_LOG(LogTemp, Warning, TEXT("I'm flying"));

        // Decrement the timer
        m_timer -= delta_time;
        UE_LOG(LogTemp, Warning, TEXT("Time Left: %f"), m_timer);

        if (m_timer <= 0)
        {
            m_current_state = BossStates::Landing;
        }
    }
    else if (m_current_state == BossStates::Landing)
    {
        UE_LOG(LogTemp, Warning, TEXT("I'm landing"));

        // Land
        FVector original_position = GetPosition();
        float new_z = original_position.Z - 600;
        SetPosition({original_position.X, original_position.Y, new_z});
        GetBoss()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);

        // Set the timer for the next state
        m_timer = 10;

        // Change the state
        m_current_state = BossStates::Resting;
    }
    else if (m_current_state == BossStates::Resting)
    {
        UE_LOG(LogTemp, Warning, TEXT("I'm resting"));

        // Decrement the timer
        m_timer -= delta_time;
        UE_LOG(LogTemp, Warning, TEXT("Time Left: %f"), m_timer);

        if (m_timer <= 0)
        {
            m_current_state = BossStates::TakingOff;
        }
    }

    // TODO: Then something to do with the pillars lol
}

void APISuccubusController::Phase3(float delta_time)
{
    ApproachPlayer(100);
    //TODO: Edit this to fit the Succubus boss
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
            RotateTowardsPlayer();
        }
    }
}

void APISuccubusController::Phase3Melee()
{
    ClearHitActors();
    StopMovement();
    //TODO: Turns these into attack chains
    if (FMath::RandRange(1, 2) == 1)
    {
        if (m_previous_attack == BossStates::Melee5)
        {
            return;
        }

        SetFocus(nullptr);

        m_current_state = BossStates::Idle;
        m_current_state = BossStates::Melee5;
        m_previous_attack = BossStates::Melee5;
    }
    else
    {
        if (m_previous_attack == BossStates::Melee3)
        {
            return;
        }

        SetFocus(nullptr);

        m_current_state = BossStates::Idle;
        m_current_state = BossStates::Melee3;
        m_previous_attack = BossStates::Melee3;
    }
}

void APISuccubusController::Teleport(int teleport_num)
{
    GetBoss()->SetActorLocation(FVector(GetBoss()->m_teleporters[teleport_num]->GetActorLocation().X, GetBoss()->m_teleporters[teleport_num]->GetActorLocation().Y, GetBoss()->GetActorLocation().Z));
}

void APISuccubusController::Phase1Melee()
{
    ClearHitActors();
    StopMovement();

    if (FMath::RandRange(1, 2) == 1)
    {
        //Replace with new enums
        //Flurry attack

        /*if (m_previous_attack == BossStates::RamAttack)
        {
            return;
        }

        SetFocus(nullptr);

        m_current_state = BossStates::Idle;
        m_current_state = BossStates::RamAttack;
        m_previous_attack = BossStates::RamAttack;*/
    }
    else
    {
        //Replace with new enums
        //Strong attack

        /*if (m_previous_attack == BossStates::SweepAttack)
        {
            return;
        }

        SetFocus(nullptr);

        m_current_state = BossStates::Idle;
        m_current_state = BossStates::SweepAttack;
        m_previous_attack = BossStates::SweepAttack;*/
    }
}
