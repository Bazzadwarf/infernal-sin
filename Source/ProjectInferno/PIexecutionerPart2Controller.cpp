// Fill out your copyright notice in the Description page of Project Settings.

#include "PIExecutionerPart2Controller.h"
//#include "HierarchicalLODOutliner/Private/HierarchicalLODType.h"

APIExecutionerPart2Controller::APIExecutionerPart2Controller()
{
    PrimaryActorTick.bCanEverTick = true;

    m_current_phase = BossPhases::Idle_Phase;
    m_current_state = BossStates::Idle;
    m_previous_state = BossStates::Idle;
}

void APIExecutionerPart2Controller::BeginPlay()
{
    Super::BeginPlay();

    m_current_phase = BossPhases::Idle_Phase;

    m_player = GetPlayer();

    m_actor_comp_array = GetBoss()->GetComponentsByTag(UBoxComponent::StaticClass(), "FallingDamageBox");
    for (UActorComponent* actor : m_actor_comp_array)
    {
        m_falling_box = Cast<UBoxComponent>(actor);
    }
}

void APIExecutionerPart2Controller::Tick(float delta_time)
{
    Super::Tick(delta_time);

    UpdateControlRotation(delta_time);

    if (!m_player_detected)
    {
        if (GetPlayerDistance() <= 3000)
        {
            m_player_detected = true;
            SetCurrentPhase();
            GetBoss()->ShowHUD();
        }
    }

    m_attack_delay += delta_time;
    if (m_attack_delay > 0.5)
    {
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
        }
    }
}

void APIExecutionerPart2Controller::SetCurrentPhase()
{
    if (GetBoss()->GetHealthPercent() > 0.7f)
    {
        m_current_phase = BossPhases::Phase_1;
        return;
    }
    if (GetBoss()->GetHealthPercent() > 0.4f)
    {
        m_current_phase = BossPhases::Phase_2;
        return;
    }
    m_current_phase = BossPhases::Phase_3;
}

BossPhases APIExecutionerPart2Controller::GetCurrentPhase()
{
    return m_current_phase;
}

BossStates APIExecutionerPart2Controller::GetCurrentState()
{
    return m_current_state;
}

void APIExecutionerPart2Controller::Idle(float delta_time)
{
    m_current_state = BossStates::Idle;
}

void APIExecutionerPart2Controller::Phase1(float delta_time)
{
    auto player_dist = GetPlayerDistance();
    if (m_ranged)
    {
        switch (m_current_state)
        {
            case BossStates::AxeStompAttack:
                m_player_far_timer = 0;
                ConeProjectiles(delta_time);
                break;
            default:
                m_ranged = false;
                break;
        }
        return;
    }
    if (RotateTowardsPlayer())
    {
        if (player_dist > 1200)
        {
            m_player_far_timer += delta_time;
        }
        else
        {
            m_player_far_timer = 0;
        }
        if (m_player_far_timer >= 4)
        {
            StopFollowing();
            m_current_state = BossStates::AxeStompAttack;
            m_previous_state = BossStates::AxeStompAttack;
            return;
        }
        if (m_previous_state != BossStates::Melee5 && player_dist < 1200 && player_dist > 900)
        {
            StopFollowing();
            m_current_state = BossStates::Melee5;
            m_previous_state = BossStates::Melee5;
        }
        else
        {
            if (player_dist > 600)
            {
                ApproachPlayer(100);
            }
            else
            {
                StopFollowing();
                m_current_state = BossStates::Melee1;
                m_previous_state = BossStates::Melee1;
            }
        }
    }
    else if (m_player_side == PlayerSide::Left && player_dist < 350 && m_previous_state != BossStates::SwipeRightToLeft
             && m_previous_state != BossStates::SwipeLeftToRight)
    {
        StopFollowing();
        m_current_state = BossStates::SwipeRightToLeft;
        m_previous_state = BossStates::SwipeRightToLeft;
    }
    else if (m_player_side == PlayerSide::Right && player_dist < 350 && m_previous_state != BossStates::SwipeLeftToRight
             && m_previous_state != BossStates::SwipeRightToLeft)
    {
        StopFollowing();
        m_current_state = BossStates::SwipeLeftToRight;
        m_previous_state = BossStates::SwipeLeftToRight;
    }
}

void APIExecutionerPart2Controller::Phase2(float delta_time)
{
    if (m_ranged)
    {
        int next_teleporter = FMath::RandRange(0, GetBoss()->m_teleporters.Num() - 1);
        switch (m_current_state)
        {
            case BossStates::AxeStompAttack:
                if (GetBoss()->m_teleporters.Num() != 0)
                {
                    GetBoss()->m_teleporters[m_current_teleporter]->SpawnLightAdd();
                }
                while (next_teleporter == m_current_teleporter)
                {
                    next_teleporter = FMath::RandRange(0, GetBoss()->m_teleporters.Num() - 1);
                }
                m_current_teleporter = next_teleporter;
                m_ranged = false;
                break;
            default:
                m_ranged = false;
                break;
        }
        return;
    }
    if (RotateTowardsTeleporter() && GetBoss()->m_teleporters.Num() != 0)
    {
        if (GetTeleporterDistance() > 700)
        {
            ApproachLocation(GetTeleporterLocation(), 100);
        }
        else
        {
            StopFollowing();
            m_current_state = BossStates::AxeStompAttack;
        }
    }
}

void APIExecutionerPart2Controller::Phase3(float delta_time)
{
    if (m_ranged)
    {
        switch (m_current_state)
        {
            case BossStates::Melee3:
                WaveAOEProjectiles(20, delta_time);
                break;
            case BossStates::Melee4:
                SpiralProjectiles(delta_time);
                break;
            case BossStates::AxeStompAttack:
                RandAOEProjectiels(25, delta_time);
                break;
            default:
                m_ranged = false;
                break;
        }
        return;
    }
    if (RotateTowardsPlayer())
    {
        auto player_dist = GetPlayerDistance();
        if (player_dist <= 400)
        {
            StopFollowing();
            m_current_state = BossStates::AxeStompAttack;
            m_previous_state = BossStates::AxeStompAttack;
        }
        else if (player_dist > 400 && player_dist < 1400 && m_previous_state != BossStates::Melee3)
        {
            StopFollowing();
            m_current_state = BossStates::Melee3;
            m_previous_state = BossStates::Melee3;
        }
        else if (player_dist > 400 && player_dist < 1200 && m_previous_state != BossStates::Melee4)
        {
            StopFollowing();
            m_current_state = BossStates::Melee4;
            m_previous_state = BossStates::Melee4;
        }
        else if (player_dist > 1300
                 || (m_previous_state == BossStates::Melee3 || m_previous_state == BossStates::Melee4))
        {
            ApproachPlayer(200);
        }
    }
}

void APIExecutionerPart2Controller::SetIdle()
{
    m_current_state = BossStates::Idle;
    m_attack_delay = 0;
    Unfocus();
    SetPlayerSide();
    ClearHitActors();
}

void APIExecutionerPart2Controller::EnableRootMotionRotation()
{
    GetBoss()->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
}

void APIExecutionerPart2Controller::DisableRootMotionRotation()
{
    GetBoss()->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
}

AProjectInfernoBoss* APIExecutionerPart2Controller::GetBoss()
{
    return Cast<AProjectInfernoBoss>(GetPawn());
}

void APIExecutionerPart2Controller::OnBossDeath()
{
    m_current_state = BossStates::Idle;
    m_current_state = BossStates::Dead;
    SetFocus(nullptr);
    StopMovement();
}

void APIExecutionerPart2Controller::Kill()
{
    GetBoss()->GetWeapon()->Destroy();
    GetBoss()->Destroy();
}

FRotator APIExecutionerPart2Controller::GetControlRotation() const
{
    if (GetPawn() == nullptr)
    {
        return {0.0f, 0.0f, 0.0f};
    }

    return {0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f};
}

AProjectInfernoPlayerCharacter* APIExecutionerPart2Controller::GetPlayer()
{
    return Cast<AProjectInfernoPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

FRotator APIExecutionerPart2Controller::GetPlayerDirection()
{
    return (GetPlayerLocation() - GetPosition()).Rotation();
}

FVector APIExecutionerPart2Controller::GetPlayerLocation()
{
    return m_player->GetActorLocation();
}

float APIExecutionerPart2Controller::GetPlayerDistance()
{
    return GetPawn()->GetDistanceTo(m_player);
}

void APIExecutionerPart2Controller::ApproachPlayer(float acceptance)
{
    MoveToActor(m_player, acceptance);
    m_current_state = BossStates::Following;
}

void APIExecutionerPart2Controller::StopFollowing()
{
    StopMovement();
    m_current_state = BossStates::Idle;
}

void APIExecutionerPart2Controller::ApproachLocation(FVector location, float acceptance)
{
    MoveToLocation(location, acceptance);
    m_current_state = BossStates::Following;
}

float APIExecutionerPart2Controller::GetTeleporterDistance()
{
    return GetPawn()->GetDistanceTo(GetBoss()->m_teleporters[m_current_teleporter]);
}

FRotator APIExecutionerPart2Controller::GetTeleporterDirection()
{
    return (GetTeleporterLocation() - GetPosition()).Rotation();
}

FVector APIExecutionerPart2Controller::GetTeleporterLocation()
{
    return GetBoss()->m_teleporters[m_current_teleporter]->GetActorLocation();
}

void APIExecutionerPart2Controller::FacePlayer()
{
    if (m_current_phase == BossPhases::Phase_2 && m_current_state == BossStates::AxeStompAttack)
    {
        return;
    }
    SetFocus(m_player, EAIFocusPriority::Gameplay);
}

void APIExecutionerPart2Controller::FacePosition(FVector position_to_look)
{
    SetFocalPoint(position_to_look, EAIFocusPriority::Gameplay);
}

FVector APIExecutionerPart2Controller::GetPosition()
{
    return GetPawn()->GetActorLocation();
}

void APIExecutionerPart2Controller::SetPosition(FVector position)
{
    GetPawn()->SetActorLocation(position);
}

void APIExecutionerPart2Controller::Fire(FVector location,
                                         FRotator rotation,
                                         TSubclassOf<class AProjectInfernoProjectile> projectile_type)
{
    FActorSpawnParameters spawn_params;
    spawn_params.Owner = this;
    spawn_params.Instigator = this->Instigator;

    auto projectile
        = GetWorld()->SpawnActor<AProjectInfernoProjectile>(projectile_type, location, rotation, spawn_params);

    if (projectile)
    {
        projectile->FireInDirection(rotation.Vector());
    }
}

void APIExecutionerPart2Controller::OnWeaponHit(UPrimitiveComponent* hit_component,
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
        case BossStates::Melee1:
        case BossStates::Melee2:
        case BossStates::SwipeLeftToRight:
        case BossStates::SwipeRightToLeft:
        case BossStates::AxeStompAttack:
            damage = weapon_properties.GetKeyValue(EPIWeaponDataKey::MeleeLightDamage).Get(0.0f);
            break;
        case BossStates::Melee3:
        case BossStates::Melee4:
        case BossStates::Melee5:
            damage = weapon_properties.GetKeyValue(EPIWeaponDataKey::MeleeHeavyDamage).Get(0.0f);
            break;
    }

    // if (weapon->GetParticleSystem() != nullptr)
    //{
    //    UGameplayStatics::SpawnEmitterAtLocation(boss,
    //                                             boss->GetWeapon()->GetParticleSystem(),
    //                                             weapon->ActorToWorld().GetLocation() + sweep_result.ImpactPoint,
    //                                             sweep_result.ImpactPoint.Rotation(),
    //                                             true);
    //}

    other_actor->TakeDamage(damage, FPointDamageEvent{}, this, weapon);
}

void APIExecutionerPart2Controller::OnFallingHit(UPrimitiveComponent* hit_component,
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
        case BossStates::Melee3:
        case BossStates::Melee4:
            damage = weapon_properties.GetKeyValue(EPIWeaponDataKey::MeleeHeavyDamage).Get(0.0f);
            break;
        default:
            return;
    }

    other_actor->TakeDamage(damage, FPointDamageEvent{}, this, weapon);
}

void APIExecutionerPart2Controller::SetPlayerSide()
{
    auto player_dir = GetPlayerDirection();

    if (FMath::Abs(player_dir.Yaw - GetControlRotation().Yaw) > 10)
    {
        if (player_dir.Yaw < 0 && GetControlRotation().Yaw < 0)
        {
            if (player_dir.Yaw < GetControlRotation().Yaw)
            {
                m_player_side = PlayerSide::Left;
            }
            else
            {
                m_player_side = PlayerSide::Right;
            }
        }
        else if (player_dir.Yaw > 0 && GetControlRotation().Yaw > 0)
        {
            if (player_dir.Yaw < GetControlRotation().Yaw)
            {
                m_player_side = PlayerSide::Left;
            }
            else
            {
                m_player_side = PlayerSide::Right;
            }
        }
        else
        {
            if (player_dir.Yaw > 0 && GetControlRotation().Yaw < 0)
            {
                if ((player_dir.Yaw + FMath::Abs(GetControlRotation().Yaw)) < 180)
                {
                    m_player_side = PlayerSide::Right;
                }
                else
                {
                    m_player_side = PlayerSide::Left;
                }
            }
            else
            {
                if ((GetControlRotation().Yaw + FMath::Abs(player_dir.Yaw)) < 180)
                {
                    m_player_side = PlayerSide::Left;
                }
                else
                {
                    m_player_side = PlayerSide::Right;
                }
            }
        }
    }
    else
    {
        m_player_side = PlayerSide::Front;
    }
}

bool APIExecutionerPart2Controller::RotateTowardsPlayer()
{
    if (m_current_state == BossStates::Idle || m_current_state == BossStates::Following)
    {
        SetPlayerSide();
        switch (m_player_side)
        {
            case PlayerSide::Left:
                m_current_state = BossStates::Following;
                GetBoss()->AddActorLocalRotation(FRotator(0, -2.5, 0));
                return false;
                ;
            case PlayerSide::Right:
                m_current_state = BossStates::Following;
                GetBoss()->AddActorLocalRotation(FRotator(0, 2.5, 0));
                return false;
                ;
            case PlayerSide::Front:
                m_current_state = BossStates::Idle;
                return true;
        }
    }
    return false;
}

void APIExecutionerPart2Controller::SetTeleporterSide()
{
    auto teleporter_dir = GetTeleporterDirection();

    if (FMath::Abs(teleporter_dir.Yaw - GetControlRotation().Yaw) > 10)
    {
        if (teleporter_dir.Yaw < 0 && GetControlRotation().Yaw < 0)
        {
            if (teleporter_dir.Yaw < GetControlRotation().Yaw)
            {
                m_teleporter_side = PlayerSide::Left;
            }
            else
            {
                m_teleporter_side = PlayerSide::Right;
            }
        }
        else if (teleporter_dir.Yaw > 0 && GetControlRotation().Yaw > 0)
        {
            if (teleporter_dir.Yaw < GetControlRotation().Yaw)
            {
                m_teleporter_side = PlayerSide::Left;
            }
            else
            {
                m_teleporter_side = PlayerSide::Right;
            }
        }
        else
        {
            if (teleporter_dir.Yaw > 0 && GetControlRotation().Yaw < 0)
            {
                if ((teleporter_dir.Yaw + FMath::Abs(GetControlRotation().Yaw)) < 180)
                {
                    m_teleporter_side = PlayerSide::Right;
                }
                else
                {
                    m_teleporter_side = PlayerSide::Left;
                }
            }
            else
            {
                if ((GetControlRotation().Yaw + FMath::Abs(teleporter_dir.Yaw)) < 180)
                {
                    m_teleporter_side = PlayerSide::Left;
                }
                else
                {
                    m_teleporter_side = PlayerSide::Right;
                }
            }
        }
    }
    else
    {
        m_teleporter_side = PlayerSide::Front;
    }
}

bool APIExecutionerPart2Controller::RotateTowardsTeleporter()
{
    if (m_current_state == BossStates::Idle || m_current_state == BossStates::Following)
    {
        SetTeleporterSide();
        switch (m_teleporter_side)
        {
            case PlayerSide::Left:
                m_current_state = BossStates::Following;
                GetBoss()->AddActorLocalRotation(FRotator(0, -2.5, 0));
                return false;
                ;
            case PlayerSide::Right:
                m_current_state = BossStates::Following;
                GetBoss()->AddActorLocalRotation(FRotator(0, 2.5, 0));
                return false;
                ;
            case PlayerSide::Front:
                m_current_state = BossStates::Idle;
                return true;
        }
    }
    return false;
}

void APIExecutionerPart2Controller::ClearHitActors()
{
    m_hit_actors.Empty();
}

void APIExecutionerPart2Controller::Unfocus()
{
    SetFocus(nullptr);
}

void APIExecutionerPart2Controller::Lunge2()
{
    SetPlayerSide();
    ClearHitActors();
    if (m_player_side == PlayerSide::Front && GetPlayerDistance() < 500)
    {
        m_current_state = BossStates::Melee2;
    }
}

void APIExecutionerPart2Controller::StartFlying()
{
    GetBoss()->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}

void APIExecutionerPart2Controller::StopFlying()
{
    GetBoss()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void APIExecutionerPart2Controller::SetRanged()
{
    auto player_dist = GetPlayerDistance();
    if (player_dist > 600 || m_staying_close
        || (m_current_phase == BossPhases::Phase_2 && m_current_state == BossStates::AxeStompAttack))
    {
        auto weapon_mesh = Cast<USkeletalMeshComponent>(GetBoss()->GetWeapon()->GetMesh());
        m_ranged_spawn_pos = weapon_mesh->GetSocketLocation("ProjectileSocket");
        m_ranged_timer = 0;
        m_ranged_counter = 0;
        m_ranged_yaw = 0;
        m_ranged = true;
    }
}

void APIExecutionerPart2Controller::EnableBodyCollider()
{
    if (m_falling_box)
    {
        m_falling_box->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }
}

void APIExecutionerPart2Controller::DisableBodyCollider()
{
    if (m_falling_box)
    {
        m_falling_box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }
}

void APIExecutionerPart2Controller::HomingProjectiles()
{
    if (m_homing_projectile && GetWorld())
    {
        Fire(m_ranged_spawn_pos, GetControlRotation(), m_homing_projectile);
    }
}

void APIExecutionerPart2Controller::SpawnerProjectiles()
{
    if (m_spawner_projectile && GetWorld())
    {
        auto rotation = GetPlayerDirection();
        rotation.Add(0, 90, 0);

        Fire(m_ranged_spawn_pos, rotation, m_spawner_projectile);
    }
}

void APIExecutionerPart2Controller::AOEProjectiles(int projectiles)
{
    if (m_slow_projectile && GetWorld())
    {
        auto rotation = FRotator{0, 0, 0};
        auto degreesInc = 360.0f / projectiles;

        for (int i = 0; i < projectiles; i++)
        {
            Fire(m_ranged_spawn_pos, {0, rotation.Yaw, 0}, m_slow_projectile);
            rotation.Add(0, degreesInc, 0);
        }
    }
}

void APIExecutionerPart2Controller::WaveAOEProjectiles(int projectiles, float delta_time)
{
    if (m_slow_projectile && GetWorld())
    {
        m_ranged_timer += delta_time;
        if (m_ranged_timer >= 0.25)
        {
            m_ranged_timer = 0;
            m_ranged_counter++;
            AOEProjectiles(projectiles);
            if (m_ranged_counter == 3)
            {
                m_ranged_counter = 0;
                m_ranged = false;
            }
        }
    }
}

void APIExecutionerPart2Controller::RandAOEProjectiels(int projectiles, float delta_time)
{
    if (m_slow_projectile && GetWorld())
    {
        m_ranged_timer += delta_time;
        if (m_ranged_timer >= 0.05)
        {
            m_ranged_timer = 0;
            m_ranged_counter++;

            Fire(m_ranged_spawn_pos, {0, FMath::RandRange(0.f, 360.f), 0}, m_slow_projectile);

            if (m_ranged_counter == projectiles)
            {
                m_ranged_counter = 0;
                m_ranged = false;
            }
        }
    }
}

void APIExecutionerPart2Controller::SpiralProjectiles(float delta_time)
{
    if (m_bounce_projectile && GetWorld())
    {
        m_ranged_timer += delta_time;
        if (m_ranged_timer >= 0.03)
        {
            m_ranged_timer = 0;
            Fire(FVector{m_ranged_spawn_pos.X, m_ranged_spawn_pos.Y, m_ranged_spawn_pos.Z + 100},
                 {0, m_ranged_yaw, 0},
                 m_bounce_projectile);
            m_ranged_yaw += 10.f;

            if (m_ranged_yaw == 360)
            {
                m_ranged_counter++;
                m_ranged_yaw = 0.f;
            }

            if (m_ranged_counter == 2)
            {
                m_ranged_counter = 0;
                m_ranged = false;
            }
        }
    }
}

void APIExecutionerPart2Controller::SwipeLeftToRight(float delta_time)
{
    if (m_rapid_projectile && GetWorld())
    {
        if (m_ranged_counter == 0)
        {
            m_ranged_yaw = 30.f;
        }

        m_ranged_timer += delta_time;
        if (m_ranged_timer >= 0.05)
        {
            m_ranged_timer = 0;
            m_ranged_counter++;

            auto weapon_mesh = Cast<USkeletalMeshComponent>(GetBoss()->GetWeapon()->GetMesh());
            m_ranged_spawn_pos = weapon_mesh->GetSocketLocation("ProjectileSocket");

            auto rotation = GetPlayerDirection();
            Fire(m_ranged_spawn_pos, {rotation.Pitch + 1, (rotation.Yaw + m_ranged_yaw), 0}, m_rapid_projectile);

            m_ranged_yaw -= 3.f;

            if (m_ranged_yaw == -30)
            {
                m_ranged_counter = 0;
                m_ranged_yaw = 0;
                m_ranged = false;
            }
        }
    }
}

void APIExecutionerPart2Controller::SwipeRightToLeft(float delta_time)
{
    if (m_rapid_projectile && GetWorld())
    {
        if (m_ranged_counter == 0)
        {
            m_ranged_yaw = -30.f;
        }

        m_ranged_timer += delta_time;
        if (m_ranged_timer >= 0.05)
        {
            m_ranged_timer = 0;
            m_ranged_counter++;

            auto weapon_mesh = Cast<USkeletalMeshComponent>(GetBoss()->GetWeapon()->GetMesh());
            m_ranged_spawn_pos = weapon_mesh->GetSocketLocation("ProjectileSocket");

            auto rotation = GetPlayerDirection();
            Fire(m_ranged_spawn_pos, {rotation.Pitch + 1, (rotation.Yaw + m_ranged_yaw), 0}, m_rapid_projectile);

            m_ranged_yaw += 3.f;

            if (m_ranged_yaw == 30)
            {
                m_ranged_counter = 0;
                m_ranged_yaw = 0;
                m_ranged = false;
            }
        }
    }
}

void APIExecutionerPart2Controller::ConeProjectiles(float delta_time)
{
    if (m_rapid_projectile && GetWorld())
    {
        if (m_ranged_counter == 0)
        {
            m_ranged_yaw = 30.f;
            m_ranged_yaw_2 = -30.f;
        }

        m_ranged_timer += delta_time;
        if (m_ranged_timer >= 0.05)
        {
            m_ranged_timer = 0;
            m_ranged_counter++;

            auto rotation = GetPlayerDirection();

            Fire(m_ranged_spawn_pos, {rotation.Pitch + 5, (rotation.Yaw + m_ranged_yaw), 0}, m_rapid_projectile);
            m_ranged_yaw -= 3.f;

            if (m_ranged_yaw == -3.f)
            {
                m_ranged_counter = 0;
                m_ranged_yaw = 0;
                m_ranged_yaw_2 = 0;
                m_ranged = false;
                return;
            }

            Fire(m_ranged_spawn_pos, {rotation.Pitch + 5, (rotation.Yaw + m_ranged_yaw_2), 0}, m_rapid_projectile);
            m_ranged_yaw_2 += 3.f;
        }
    }
}

void APIExecutionerPart2Controller::FrontalBarrage(float delta_time)
{
    if (m_regular_projectile && GetWorld())
    {
        m_ranged_timer += delta_time;
        if (m_ranged_timer >= 0.05)
        {
            m_ranged_timer = 0;
            m_ranged_counter++;

            auto rotation = GetPlayerDirection();
            Fire(m_ranged_spawn_pos,
                 {rotation.Pitch + FMath::RandRange(-2.f, 6.f), (rotation.Yaw + FMath::RandRange(-8.f, 8.f)), 0},
                 m_regular_projectile);

            if (m_ranged_counter == 60)
            {
                m_ranged_counter = 0;
                m_ranged = false;
            }
        }
    }
}
