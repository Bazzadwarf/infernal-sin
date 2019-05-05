// Fill out your copyright notice in the Description page of Project Settings.

#include "PIBossController.h"

APIBossController::APIBossController()
{
    PrimaryActorTick.bCanEverTick = true;

    m_sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

    m_sight_config->SightRadius = m_detection_radius;
    m_sight_config->LoseSightRadius = m_undetect_radius;
    m_sight_config->PeripheralVisionAngleDegrees = m_peripheral_vision_angle;
    m_sight_config->SetMaxAge(m_detection_age);

    m_sight_config->DetectionByAffiliation.bDetectEnemies = true;
    m_sight_config->DetectionByAffiliation.bDetectFriendlies = true;
    m_sight_config->DetectionByAffiliation.bDetectNeutrals = true;

    // Set dominant sense as sight
    GetPerceptionComponent()->SetDominantSense(*m_sight_config->GetSenseImplementation());

    // When a pawn is detected
    GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &APIBossController::PawnDetected);
    GetPerceptionComponent()->ConfigureSense(*m_sight_config);

    m_current_phase = BossPhases::Idle_Phase;
    m_current_state = BossStates::Idle;
    m_previous_attack = BossStates::Idle;
}

void APIBossController::BeginPlay()
{
    Super::BeginPlay();
}

void APIBossController::Tick(float delta_time)
{
    Super::Tick(delta_time);

    UpdateControlRotation(delta_time);

    if (m_player_detected)
    {
        m_current_phase = GetCurrentPhase();
    }
}

void APIBossController::Possess(APawn* pawn)
{
    Super::Possess(pawn);
}

FRotator APIBossController::GetControlRotation() const
{
    if (GetPawn() == nullptr)
    {
        return {0.0f, 0.0f, 0.0f};
    }

    return {0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f};
}

void APIBossController::PawnDetected(const TArray<AActor*>& pawns)
{
    for (auto pawn : pawns)
    {
        if (auto player = Cast<AProjectInfernoPlayerCharacter>(pawn))
        {
            m_player_detected = true;
        }
    }
}

AProjectInfernoBoss* APIBossController::GetBoss()
{
    return Cast<AProjectInfernoBoss>(GetPawn());
}

void APIBossController::OnBossDeath()
{
    GetWorldTimerManager().ClearTimer(m_fire_delay_handle);
    m_current_state = BossStates::Idle;
    m_current_state = BossStates::Dead;
    SetFocus(nullptr);
    StopMovement();
}

void APIBossController::Kill()
{
    GetBoss()->GetWeapon()->Destroy();
    GetBoss()->Destroy();
}

BossPhases APIBossController::GetCurrentPhase()
{
    return BossPhases::Idle_Phase;
}

void APIBossController::Idle(float delta_time)
{
    m_current_state = BossStates::Idle;
}

void APIBossController::Phase1(float delta_time)
{
}

void APIBossController::Phase2(float delta_time)
{
}

void APIBossController::Phase3(float delta_time)
{
}

void APIBossController::Phase4(float delta_time)
{
}

AProjectInfernoPlayerCharacter* APIBossController::GetPlayer()
{
    return Cast<AProjectInfernoPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

FRotator APIBossController::GetPlayerDirection()
{
    return (GetPlayer()->GetActorLocation() - GetPosition()).Rotation();
}

float APIBossController::GetPlayerDistance()
{
    return GetPawn()->GetDistanceTo(GetPlayer());
}

void APIBossController::ApproachPlayer(float acceptance)
{
    // FString TheFloatStr = FString::SanitizeFloat(m_distance_to_player);
    // GEngine->AddOnScreenDebugMessage( -1,1.0,FColor::Red, TheFloatStr);

    MoveToActor(GetPlayer(), acceptance);
    m_current_state = BossStates::Following;
}

void APIBossController::ApproachLocation(FVector location, float acceptance)
{
    MoveToLocation(location, acceptance);
}

void APIBossController::FacePlayer()
{
    SetFocus(GetPlayer(), EAIFocusPriority::Gameplay);
}

void APIBossController::FacePosition(FVector position_to_look)
{
    SetFocalPoint(position_to_look, EAIFocusPriority::Gameplay);
}

FVector APIBossController::GetPosition()
{
    return GetPawn()->GetActorLocation();
}

void APIBossController::IsAttacking(bool is_attacking)
{
    m_attacking = is_attacking;
}

void APIBossController::SetIdle()
{
    m_current_state = BossStates::Idle;
    m_hit_actors.Empty();
}

BossStates APIBossController::GetCurrentState()
{
    return m_current_state;
}

void APIBossController::HomingProjectiles()
{
    if (m_homing_projectile && GetWorld())
    {
        FActorSpawnParameters spawn_params;
        spawn_params.Owner = this;
        spawn_params.Instigator = this->Instigator;

        auto location = GetBoss()->GetWeapon()->GetMesh()->GetSocketLocation("CenterSocket");
        Fire(location, GetControlRotation(), spawn_params, m_homing_projectile);
    }
}

void APIBossController::CharmProjectiles()
{
    if (m_charm_projectile && GetWorld())
    {
        FActorSpawnParameters spawn_params;
        spawn_params.Owner = this;
        spawn_params.Instigator = this->Instigator;

        auto location = GetPosition() + FVector(0, 0, 0);

        Fire(location, GetControlRotation(), spawn_params, m_charm_projectile);
    }
}

void APIBossController::RapidProjectiles()
{
    if (m_rapid_projectile && GetWorld())
    {
        FActorSpawnParameters spawn_params;
        spawn_params.Owner = this;
        spawn_params.Instigator = this->Instigator;

        Fire(GetPosition(), GetControlRotation(), spawn_params, m_rapid_projectile);
    }
}

void APIBossController::SpawnerProjectiles()
{
    if (m_spawner_projectile && GetWorld())
    {
        FActorSpawnParameters spawn_params;
        spawn_params.Owner = this;
        spawn_params.Instigator = this->Instigator;

        auto rotation = GetPlayerDirection();
        rotation.Add(0, 90, 0);

        Fire(GetPosition(), rotation, spawn_params, m_spawner_projectile);
    }
}

void APIBossController::AOEProjectiles()
{
    if (m_slow_projectile && GetWorld())
    {
        FActorSpawnParameters spawn_params;
        spawn_params.Owner = this;
        spawn_params.Instigator = this->Instigator;

        auto rotation = GetPlayerDirection();

        for (int i = 0; i < 24; i++)
        {
            Fire(GetPosition(), {rotation.Pitch + 1, rotation.Yaw, 0}, spawn_params, m_slow_projectile);
            rotation.Add(0, 15, 0);
        }
    }
}

void APIBossController::SpiralProjectiles()
{
    if (m_bounce_projectile && GetWorld())
    {
        if (!GetWorldTimerManager().TimerExists(m_fire_delay_handle))
        {
            auto attack_callback = [&, yaw_rotation = 0.f, count = 0]() mutable {
                FActorSpawnParameters spawn_params;
                spawn_params.Owner = this;
                spawn_params.Instigator = this->Instigator;

                Fire(GetPosition(), {0, yaw_rotation, 0}, spawn_params, m_bounce_projectile);

                yaw_rotation += 10.f;

                if (yaw_rotation == 360)
                {
                    count++;
                    yaw_rotation = 0.f;

                    if (count == 2)
                    {
                        GetWorldTimerManager().ClearTimer(m_fire_delay_handle);
                    }
                }
            };

            GetWorldTimerManager().SetTimer(m_fire_delay_handle, attack_callback, 0.025f, true);
        }
    }
}

void APIBossController::ConeProjectiles()
{
    if (m_rapid_projectile && GetWorld())
    {
        if (!GetWorldTimerManager().TimerExists(m_fire_delay_handle))
        {
            auto rotation = GetPlayerDirection();

            auto attack_callback = [&, yaw_rotation1 = 30.f, yaw_rotation2 = -30.f, rotation]() mutable {
                FActorSpawnParameters spawn_params;
                spawn_params.Owner = this;
                spawn_params.Instigator = this->Instigator;

                Fire(GetPosition(),
                     {rotation.Pitch + 1, (rotation.Yaw + yaw_rotation1), 0},
                     spawn_params,
                     m_rapid_projectile);

                if (yaw_rotation2 != 3)
                {
                    Fire(GetPosition(),
                         {rotation.Pitch + 1, (rotation.Yaw + yaw_rotation2), 0},
                         spawn_params,
                         m_rapid_projectile);
                }

                yaw_rotation1 -= 3.f;
                yaw_rotation2 += 3.f;

                if (yaw_rotation1 == -3)
                {
                    GetWorldTimerManager().ClearTimer(m_fire_delay_handle);
                }
            };

            GetWorldTimerManager().SetTimer(m_fire_delay_handle, attack_callback, 0.05f, true);
        }
    }
}

void APIBossController::ConeProjectilesReverse()
{
    if (m_rapid_projectile && GetWorld())
    {
        if (!GetWorldTimerManager().TimerExists(m_fire_delay_handle))
        {
            FActorSpawnParameters spawn_params;
            spawn_params.Owner = this;
            spawn_params.Instigator = this->Instigator;

            auto rotation = GetPlayerDirection();

            auto attack_callback = [&, yaw_rotation1 = 3.f, yaw_rotation2 = -3.f, rotation, spawn_params]() mutable {
                Fire(GetPosition(),
                     {rotation.Pitch + 1, (rotation.Yaw + yaw_rotation1), 0},
                     spawn_params,
                     m_rapid_projectile);

                Fire(GetPosition(),
                     {rotation.Pitch + 1, (rotation.Yaw + yaw_rotation2), 0},
                     spawn_params,
                     m_rapid_projectile);

                yaw_rotation1 += 3.f;
                yaw_rotation2 -= 3.f;

                if (yaw_rotation1 == 30)
                {
                    GetWorldTimerManager().ClearTimer(m_fire_delay_handle);
                }
            };

            Fire(GetPosition(), {rotation.Pitch + 1, rotation.Yaw, 0}, spawn_params, m_rapid_projectile);
            GetWorldTimerManager().SetTimer(m_fire_delay_handle, attack_callback, 0.05f, true);
        }
    }
}

void APIBossController::Fire(FVector location,
                             FRotator rotation,
                             FActorSpawnParameters spawn_params,
                             TSubclassOf<class AProjectInfernoProjectile> projectile_type)
{
    auto projectile
        = GetWorld()->SpawnActor<AProjectInfernoProjectile>(projectile_type, location, rotation, spawn_params);

    if (projectile)
    {
        projectile->FireInDirection(rotation.Vector());
    }
}

void APIBossController::OnWeaponHit(UPrimitiveComponent* hit_component,
                                    AActor* other_actor,
                                    UPrimitiveComponent* other_component,
                                    int32 other_body_index,
                                    bool b_from_sweep,
                                    const FHitResult& sweep_result)
{
    auto boss = GetBoss();
    auto weapon = boss->GetWeapon();

    if (other_actor == nullptr || other_actor == boss || m_hit_actors.Contains(other_actor))
    {
        return;
    }

    m_hit_actors.Add(other_actor);

    const auto& weapon_properties = weapon->GetWeaponProperties();

    float damage = 0.0f;

    switch (m_current_state)
    {
        case BossStates::LeftAttack:
        case BossStates::RightAttack:
        case BossStates::RamAttack:
        case BossStates::AxeStompAttack:
            damage = weapon_properties.GetKeyValue(EPIWeaponDataKey::MeleeLightDamage).Get(0.0f);
            break;
        case BossStates::OverheadAttack:
        case BossStates::SweepAttack:
            damage = weapon_properties.GetKeyValue(EPIWeaponDataKey::MeleeHeavyDamage).Get(0.0f);
            break;
    }

    other_actor->TakeDamage(damage, FPointDamageEvent{}, this, weapon);
}

bool APIBossController::RotateTowardsPlayer()
{
    if (m_current_state == BossStates::Idle || m_current_state == BossStates::Following)
    {
        if (FMath::Abs(GetPlayerDirection().Yaw - GetControlRotation().Yaw) > 15)
        {
            m_current_state = BossStates::Following;

            if (GetPlayerDirection().Yaw < 0 && GetControlRotation().Yaw < 0)
            {
                if (GetPlayerDirection().Yaw < GetControlRotation().Yaw)
                {
                    m_player_side = PlayerSide::Left;
                    GetBoss()->AddActorLocalRotation(FRotator(0, -2.5, 0));
                }
                else
                {
                    m_player_side = PlayerSide::Right;
                    GetBoss()->AddActorLocalRotation(FRotator(0, 2.5, 0));
                }
            }
            else if (GetPlayerDirection().Yaw > 0 && GetControlRotation().Yaw > 0)
            {
                if (GetPlayerDirection().Yaw < GetControlRotation().Yaw)
                {
                    m_player_side = PlayerSide::Left;
                    GetBoss()->AddActorLocalRotation(FRotator(0, -2.5, 0));
                }
                else
                {
                    m_player_side = PlayerSide::Right;
                    GetBoss()->AddActorLocalRotation(FRotator(0, 2.5, 0));
                }
            }
            else
            {
                if (GetPlayerDirection().Yaw > 0 && GetControlRotation().Yaw < 0)
                {
                    if ((GetPlayerDirection().Yaw + FMath::Abs(GetControlRotation().Yaw)) < 180)
                    {
                        m_player_side = PlayerSide::Right;
                        GetBoss()->AddActorLocalRotation(FRotator(0, 2.5, 0));
                    }
                    else
                    {
                        m_player_side = PlayerSide::Left;
                        GetBoss()->AddActorLocalRotation(FRotator(0, -2.5, 0));
                    }
                }
                else
                {
                    if ((GetControlRotation().Yaw + FMath::Abs(GetPlayerDirection().Yaw)) < 180)
                    {
                        m_player_side = PlayerSide::Left;
                        GetBoss()->AddActorLocalRotation(FRotator(0, -2.5, 0));
                    }
                    else
                    {
                        m_player_side = PlayerSide::Right;
                        GetBoss()->AddActorLocalRotation(FRotator(0, 2.5, 0));
                    }
                }
            }
        }
        else
        {
            m_player_side = PlayerSide::Front;
            m_current_state = BossStates::Idle;
            return true;
        }
    }

    return false;
}

void APIBossController::ClearHitActors()
{
    m_hit_actors.Empty();
}
