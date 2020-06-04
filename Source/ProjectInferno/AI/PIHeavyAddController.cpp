// Fill out your copyright notice in the Description page of Project Settings.

#include "PIHeavyAddController.h"

APIHeavyAddController::APIHeavyAddController(const FObjectInitializer& object_initializer)
{
    PrimaryActorTick.bCanEverTick = true;

    m_current_state = HeavyStates::Idle;
    m_previous_state = HeavyStates::Idle;
}

void APIHeavyAddController::BeginPlay()
{
    Super::BeginPlay();

    m_player = GetPlayer();

    GetHeavyAdd()->GetLeftHandCollider()->OnComponentBeginOverlap.AddDynamic(this, &APIHeavyAddController::OnMeleeHit);
    GetHeavyAdd()->GetRightHandCollider()->OnComponentBeginOverlap.AddDynamic(this, &APIHeavyAddController::OnMeleeHit);
    GetHeavyAdd()->GetBodyCollider()->OnComponentBeginOverlap.AddDynamic(this, &APIHeavyAddController::OnMeleeHit);
}

void APIHeavyAddController::Tick(float delta_time)
{
    UpdateControlRotation(delta_time);

    m_damage_maintained_timer += delta_time;
    if (m_damage_maintained_timer >= 4)
    {
        m_cumulated_damage = 0;
    }

    if (!m_player_detected)
    {
        Idle(delta_time);
        if (GetPlayerDistance() <= 3000)
        {
            m_player_detected = true;
        }
    }
    else if (!m_is_stunned)
    {
        Attack(delta_time);
    }
}

HeavyStates APIHeavyAddController::GetCurrentState()
{
    return m_current_state;
}

void APIHeavyAddController::Idle(float delta_time)
{
    m_current_state = HeavyStates::Idle;
}

void APIHeavyAddController::Attack(float delta_time)
{
    auto player_dist = GetPlayerDistance();
    if (player_dist < 600)
    {
        m_stay_close_timer += delta_time;
    }
    else
    {
        m_stay_close_timer = 0;
    }

    if (player_dist > 1000)
    {
        m_player_far_timer += delta_time;
    }
    else
    {
        m_player_far_timer = 0;
    }

    if (m_ranged)
    {
        switch (m_current_state)
        {
            case HeavyStates::Stomp:
                RandAOEProjectiles(40, delta_time);
                break;
            case HeavyStates::Swipe:
                FrontalBarrage(delta_time);
                break;
            case HeavyStates::Charge1:
                WaveAOEProjectiles(18, delta_time);
                break;
            default:
                m_ranged = false;
                break;
        }
        return;
    }
    if (RotateTowardsPlayer())
    {
        if (m_stay_close_timer >= 6)
        {
            m_stay_close_timer = 0;
            StopFollowing();
            m_current_state = HeavyStates::Stomp;
            m_previous_state = HeavyStates::Stomp;
            return;
        }
        if (m_player_far_timer >= 4)
        {
            StopFollowing();
            m_current_state = HeavyStates::Swipe;
            m_previous_state = HeavyStates::Swipe;
            return;
        }
        if (m_previous_state != HeavyStates::Charge1 && player_dist < 1200 && player_dist > 700)
        {
            StopFollowing();
            m_current_state = HeavyStates::Charge1;
            m_previous_state = HeavyStates::Charge1;
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
                m_current_state = HeavyStates::Swipe;
                m_previous_state = HeavyStates::Swipe;
            }
        }
    }
}

void APIHeavyAddController::SetIdle()
{
    m_current_state = HeavyStates::Idle;
    Unfocus();
    SetPlayerSide();
    ClearHitActors();
}

void APIHeavyAddController::EnableRootMotionRotation()
{
    GetHeavyAdd()->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
}

void APIHeavyAddController::DisableRootMotionRotation()
{
    GetHeavyAdd()->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
}

APIHeavyAdd* APIHeavyAddController::GetHeavyAdd()
{
    return Cast<APIHeavyAdd>(GetPawn());
}

void APIHeavyAddController::OnDamaged(FPIDamageInfo info)
{
    m_cumulated_damage += info.damage;
    m_damage_maintained_timer = 0;
    if (m_cumulated_damage >= 140)
    {
        m_is_stunned = true;
        if (m_cumulated_damage >= 180)
        {
            if (m_cumulated_damage >= 220)
            {
                m_current_state = HeavyStates::Knockback3;
                return;
            }
            m_current_state = HeavyStates::Knockback2;
            return;
        }
        m_current_state = HeavyStates::Knockback1;
    }
}

void APIHeavyAddController::StopStun()
{
    m_cumulated_damage = 0;
    m_is_stunned = false;
    m_current_state = HeavyStates::Idle;
}

void APIHeavyAddController::OnDeath(FPIDamageInfo info)
{
    m_current_state = HeavyStates::Dead;
    SetFocus(nullptr);
    StopMovement();

    GetHeavyAdd()->GetLeftHandCollider()->OnComponentBeginOverlap.RemoveDynamic(this,
                                                                                &APIHeavyAddController::OnMeleeHit);
    GetHeavyAdd()->GetRightHandCollider()->OnComponentBeginOverlap.RemoveDynamic(this,
                                                                                 &APIHeavyAddController::OnMeleeHit);
    GetHeavyAdd()->GetBodyCollider()->OnComponentBeginOverlap.RemoveDynamic(this,
                                                                            &APIHeavyAddController::OnMeleeHit);
}

void APIHeavyAddController::Kill()
{
    GetHeavyAdd()->Dead();
}

FRotator APIHeavyAddController::GetControlRotation() const
{
    if (GetPawn() == nullptr)
    {
        return {0.0f, 0.0f, 0.0f};
    }

    return {0.0f, GetPawn()->GetActorRotation().Yaw, 0.0f};
}

AProjectInfernoPlayerCharacter* APIHeavyAddController::GetPlayer()
{
    return Cast<AProjectInfernoPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}

FRotator APIHeavyAddController::GetPlayerDirection()
{
    return (GetPlayerLocation() - GetPosition()).Rotation();
}

FVector APIHeavyAddController::GetPlayerLocation()
{
    return m_player->GetActorLocation();
}

float APIHeavyAddController::GetPlayerDistance()
{
    return GetPawn()->GetDistanceTo(m_player);
}

void APIHeavyAddController::ApproachPlayer(float acceptance)
{
    MoveToActor(m_player, acceptance);
    m_current_state = HeavyStates::Moving;
}

void APIHeavyAddController::StopFollowing()
{
    StopMovement();
    m_current_state = HeavyStates::Idle;
}

void APIHeavyAddController::FacePlayer()
{
    SetFocus(m_player, EAIFocusPriority::Gameplay);
}

FVector APIHeavyAddController::GetPosition()
{
    return GetPawn()->GetActorLocation();
}

void APIHeavyAddController::OnMeleeHit(UPrimitiveComponent* hit_component,
                                       AActor* other_actor,
                                       UPrimitiveComponent* other_component,
                                       int32 other_body_index,
                                       bool b_from_sweep,
                                       const FHitResult& sweep_result)
{
    auto heavy_add = GetHeavyAdd();

    if (other_actor == nullptr || other_actor == heavy_add || m_hit_actors.Contains(other_actor))
    {
        return;
    }

    m_hit_actors.Add(other_actor);

    float damage = 0.0f;

    switch (m_current_state)
    {
        case HeavyStates::Swipe:
        case HeavyStates::Backswipe:
            damage = 15;
            break;
        case HeavyStates::Charge2:
            damage = 25;
            break;
        default:
            return;
    }

    other_actor->TakeDamage(damage, FPointDamageEvent{}, this, heavy_add);
}

void APIHeavyAddController::Fire(FVector location,
                                 FRotator rotation,
                                 TSubclassOf<class AProjectInfernoProjectile> projectile_type)
{
    FActorSpawnParameters spawn_params;
    spawn_params.Owner = this;
    spawn_params.Instigator = this->GetInstigator();

    auto projectile
        = GetWorld()->SpawnActor<AProjectInfernoProjectile>(projectile_type, location, rotation, spawn_params);

    if (projectile)
    {
        projectile->FireInDirection(rotation.Vector());
    }
}

void APIHeavyAddController::SetPlayerSide()
{
    auto player_dir = GetPlayerDirection();
    auto Add_rot = GetControlRotation();

    auto degrees = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Add_rot.Vector(), player_dir.Vector())));

    if (degrees > 70)
    {
        m_player_at_back = true;
    }
    else
    {
        m_player_at_back = false;
        if (degrees < 20)
        {
            m_player_side = PlayersSide::Front;
            return;
        }
    }

    if (FMath::Abs(player_dir.Yaw - Add_rot.Yaw) > 10)
    {
        if (player_dir.Yaw < 0 && Add_rot.Yaw < 0)
        {
            if (player_dir.Yaw < Add_rot.Yaw)
            {
                m_player_side = PlayersSide::Left;
            }
            else
            {
                m_player_side = PlayersSide::Right;
            }
        }
        else if (player_dir.Yaw > 0 && Add_rot.Yaw > 0)
        {
            if (player_dir.Yaw < Add_rot.Yaw)
            {
                m_player_side = PlayersSide::Left;
            }
            else
            {
                m_player_side = PlayersSide::Right;
            }
        }
        else
        {
            if (player_dir.Yaw > 0 && Add_rot.Yaw < 0)
            {
                if ((player_dir.Yaw + FMath::Abs(Add_rot.Yaw)) < 180)
                {
                    m_player_side = PlayersSide::Right;
                }
                else
                {
                    m_player_side = PlayersSide::Left;
                }
            }
            else
            {
                if ((Add_rot.Yaw + FMath::Abs(player_dir.Yaw)) < 180)
                {
                    m_player_side = PlayersSide::Left;
                }
                else
                {
                    m_player_side = PlayersSide::Right;
                }
            }
        }
    }
    else
    {
        m_player_side = PlayersSide::Front;
    }
}

bool APIHeavyAddController::RotateTowardsPlayer()
{
    if (m_current_state == HeavyStates::Idle || m_current_state == HeavyStates::Moving)
    {
        SetPlayerSide();
        switch (m_player_side)
        {
            case PlayersSide::Left:
                m_current_state = HeavyStates::Moving;
                GetHeavyAdd()->AddActorLocalRotation(FRotator(0, -2.5, 0));
                return false;
            case PlayersSide::Right:
                m_current_state = HeavyStates::Moving;
                GetHeavyAdd()->AddActorLocalRotation(FRotator(0, 2.5, 0));
                return false;
            case PlayersSide::Front:
                m_current_state = HeavyStates::Idle;
                return true;
        }
    }
    return false;
}

void APIHeavyAddController::ClearHitActors()
{
    m_hit_actors.Empty();
}

void APIHeavyAddController::Unfocus()
{
    SetFocus(nullptr);
}

void APIHeavyAddController::StartCharge()
{
    m_current_state = HeavyStates::Charge2;
    m_previous_state = HeavyStates::Charge2;
}

void APIHeavyAddController::EndCharge()
{
    m_current_state = HeavyStates::Charge3;
    m_previous_state = HeavyStates::Charge3;
}

void APIHeavyAddController::SetRanged()
{
    auto player_dist = GetPlayerDistance();
    if (player_dist > 600 || m_current_state == HeavyStates::Stomp)
    {
        USkeletalMeshComponent* heavy_mesh = Cast<USkeletalMeshComponent>(GetHeavyAdd()->GetMesh());
        switch (m_current_state)
        {
            case HeavyStates::Stomp:
                m_ranged_spawn_pos = heavy_mesh->GetSocketLocation("FootSocket");
                break;
            case HeavyStates::Swipe:
                m_ranged_spawn_pos = heavy_mesh->GetSocketLocation("RightHandSocket");
                break;
            case HeavyStates::Charge1:
                m_ranged_spawn_pos = heavy_mesh->GetSocketLocation("CenterFootSocket");
                break;
            default:
                return;
        }
        m_ranged_timer = 0;
        m_ranged_counter = 0;
        m_ranged = true;
    }
}

void APIHeavyAddController::EnableLeftHandCollider()
{
    GetHeavyAdd()->EnableLeftHandColliders();
}

void APIHeavyAddController::EnableRightHandCollider()
{
    GetHeavyAdd()->EnableRightHandColliders();
}

void APIHeavyAddController::EnableBodyCollider()
{
    GetHeavyAdd()->EnableBodyCollider();
}

void APIHeavyAddController::DisableAllColliders()
{
    GetHeavyAdd()->DisableLeftHandColliders();
    GetHeavyAdd()->DisableRightHandColliders();
    GetHeavyAdd()->DisableBodyCollider();
}

void APIHeavyAddController::AOEProjectiles(int projectiles)
{
    if (m_regular_projectile && GetWorld())
    {
        auto rotation = FRotator{0, 0, 0};
        auto degreesInc = 360.0f / projectiles;

        for (int i = 0; i < projectiles; i++)
        {
            Fire(m_ranged_spawn_pos, {0, rotation.Yaw, 0}, m_regular_projectile);
            rotation.Add(0, degreesInc, 0);
        }
    }
}

void APIHeavyAddController::WaveAOEProjectiles(int projectiles, float delta_time)
{
    if (m_regular_projectile && GetWorld())
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

void APIHeavyAddController::RandAOEProjectiles(int projectiles, float delta_time)
{
    if (m_regular_projectile && GetWorld())
    {
        m_ranged_timer += delta_time;
        if (m_ranged_timer >= 0.05)
        {
            m_ranged_timer = 0;
            m_ranged_counter++;

            Fire(m_ranged_spawn_pos, {0, FMath::RandRange(0.f, 360.f), 0}, m_regular_projectile);

            if (m_ranged_counter == projectiles)
            {
                m_ranged_counter = 0;
                m_ranged = false;
            }
        }
    }
}

void APIHeavyAddController::FrontalBarrage(float delta_time)
{
    if (m_regular_projectile && GetWorld())
    {
        m_ranged_timer += delta_time;
        if (m_ranged_timer >= 0.05)
        {
            m_ranged_timer = 0;
            m_ranged_counter++;

            auto rotation = (GetPlayerLocation() - m_ranged_spawn_pos).Rotation();
            Fire(m_ranged_spawn_pos,
                 {FMath::RandRange(-15.f, -5.f), (rotation.Yaw + FMath::RandRange(-12.f, 12.f)), 0},
                 m_regular_projectile);

            if (m_ranged_counter == 30)
            {
                m_ranged_counter = 0;
                m_ranged = false;
            }
        }
    }
}
