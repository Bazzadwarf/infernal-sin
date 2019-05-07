// Fill out your copyright notice in the Description page of Project Settings.

#include "PIPlayerController.h"
#include "AnimNotifies/PICharacterEquipWeaponNotify.h"
#include "Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "PICollisionChannels.h"
#include "PIHealthComponent.h"
#include "ProjectInfernoPlayerCharacter.h"
#include "ProjectInfernoProjectile.h"
#include "UserWidget.h"
#include "WidgetBlueprintLibrary.h"

void APIPlayerController::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    PlayerCameraManager->ViewPitchMax = m_view_pitch_max;
    PlayerCameraManager->ViewPitchMin = m_view_pitch_min;
}

void APIPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    using Controller = APIPlayerController;

    InputComponent->BindAxis("MoveForward", this, &Controller::MoveForward);
    InputComponent->BindAxis("MoveSideways", this, &Controller::MoveSideways);

    InputComponent->BindAction("Sprint", IE_Pressed, this, &Controller::StartSprinting);
    InputComponent->BindAction("Sprint", IE_Released, this, &Controller::StopSprinting);

    InputComponent->BindAction("Dodge", IE_Pressed, this, &Controller::StartDodge);

    // Camera rotation controls for mouse
    InputComponent->BindAxis("Turn", this, &Controller::AddYawInput);
    InputComponent->BindAxis("LookUp", this, &Controller::AddPitchInput);

    // Camera rotation controls for joysticks
    InputComponent->BindAxis("TurnRate", this, &Controller::TurnAtRate);
    InputComponent->BindAxis("LookUpRate", this, &Controller::LookUpAtRate);

    InputComponent->BindAction("Light Attack", IE_Pressed, this, &Controller::PerformLightAttack);
    InputComponent->BindAction("Heavy Attack", IE_Pressed, this, &Controller::PerformHeavyAttack);

    InputComponent->BindAction("Light Projectile Attack", IE_Pressed, this, &Controller::PerformLightProjectileAttack);
    InputComponent->BindAction("Heavy Projectile Attack", IE_Pressed, this, &Controller::PerformHeavyProjectileAttack);

    InputComponent->BindAction("Pause", IE_Pressed, this, &Controller::Pause);
}

void APIPlayerController::BeginPlay()
{
    Super::BeginPlay();

    // Create pause menu widget
    if (m_pause_menu_class)
    {
        m_pause_menu_widget = CreateWidget<UUserWidget>(GetWorld(), m_pause_menu_class);
    }

    // Displays character health bar
    if (m_player_ui_class != nullptr)
    {
        m_player_ui_widget = CreateWidget<UUserWidget>(GetWorld(), m_player_ui_class);

        if (m_player_ui_widget != nullptr)
        {
            m_player_ui_widget->AddToViewport();
        }
    }
}

void APIPlayerController::Possess(APawn* pawn)
{
    Super::Possess(pawn);

    if (pawn)
    {
        if (auto health_component
            = Cast<UPIHealthComponent>(pawn->GetComponentByClass(UPIHealthComponent::StaticClass())))
        {
            health_component->OnDamage.BindUFunction(this, "OnPlayerDamaged");
        }
    }
}

void APIPlayerController::UnPossess()
{
    GetWorld()->GetTimerManager().ClearTimer(m_charm_timer_handle);
}

void APIPlayerController::Tick(float delta_time)
{
    Super::Tick(delta_time);

    auto player = GetPlayerCharacter();

    if (!m_charmed)
    {
        if (IsDodging())
        {
            auto direction = player->GetActorForwardVector();
            player->SetActorLocation(player->GetActorLocation()
                                     + (direction * m_dodge_distance_per_second * delta_time));
        }
    }

    if (m_invulnerability_time_left > 0.0f)
    {
        m_invulnerability_time_left -= delta_time;
    }
}

void APIPlayerController::Charm()
{
    m_charmed = true;
    auto player = GetPlayerCharacter();
    player->GetWeapon()->AttachToComponent(
        GetPlayerCharacter()->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponBack");

    auto callback = [&]() {
        m_charmed = false;
        GetWorldTimerManager().ClearTimer(m_charm_timer_handle);
    };

    GetWorldTimerManager().SetTimer(m_charm_timer_handle, callback, 3.0f, false);
}

void APIPlayerController::OnWeaponHit(UPrimitiveComponent* hit_component,
                                      AActor* other_actor,
                                      UPrimitiveComponent* other_component,
                                      int32 other_body_index,
                                      bool b_from_sweep,
                                      const FHitResult& sweep_result)
{
    auto player = GetPlayerCharacter();
    auto weapon = player->GetWeapon();

    if (other_actor == player || m_hit_actors.Contains(other_actor)
        || !other_actor->FindComponentByClass<UPIHealthComponent>())
    {
        return;
    }

    m_hit_actors.Add(other_actor);

    const auto& weapon_properties = weapon->GetWeaponProperties();

    float damage = 0.0f;

    if (m_last_attack == EPIPlayerAttack::LightMelee)
    {
        if (auto value = weapon_properties.GetKeyValue(EPIWeaponDataKey::MeleeLightDamage))
        {
            damage = value.GetValue();
            player->RegenerateProjectile(0.2f);
        }
    }

    if (m_last_attack == EPIPlayerAttack::HeavyMelee)
    {
        if (auto value = weapon_properties.GetKeyValue(EPIWeaponDataKey::MeleeHeavyDamage))
        {
            damage = value.GetValue();
            player->RegenerateProjectile(0.4f);
        }
    }

    if (player->GetWeapon()->GetParticleSystem() != nullptr)
    {
        UGameplayStatics::SpawnEmitterAtLocation(player,
                                                 player->GetWeapon()->GetParticleSystem(),
                                                 weapon->ActorToWorld().GetLocation() + sweep_result.ImpactPoint,
                                                 sweep_result.ImpactPoint.Rotation(),
                                                 true);
    }

    other_actor->TakeDamage(damage, FPointDamageEvent{}, this, weapon);
}

void APIPlayerController::OnPlayerDamaged(FPIDamageInfo info)
{
    auto player = GetPlayerCharacter();

    if (player->GetHealth() > 0)
    {
        if (m_invulnerability_time_left > 0.0f)
        {
            player->GetHealthComponent()->RecoverHealth(info.damage);
            return;
        }

        if (m_accept_attack_input)
        {
            // Orient the player to the damage source otherwise the root motion in the animation will move us in
            // backwards regardless of the damage source location
            const auto player_loc = player->GetActorLocation();
            const auto damage_loc = info.damage_causer->GetActorLocation();
            const auto rotation_to_add = UKismetMathLibrary::FindLookAtRotation(player_loc, damage_loc).Yaw;

            auto current_rot = player->GetActorRotation();
            current_rot.Add(0.0f, rotation_to_add, 0.0f);
            player->SetActorRotation(current_rot);

            m_accept_movement_input = false;
            m_accept_attack_input = false;
            StopCurrentAttackChain();
            GetPlayerCharacter()->PlayAnimMontage(m_knockback_animation);

            m_invulnerability_time_left = 0.9f;
        }
    }
}

void APIPlayerController::StopKnockbackAnimation()
{
    m_accept_movement_input = true;
    m_accept_attack_input = true;
}

void APIPlayerController::Pause()
{
    if (this->SetPause(true))
    {
        if (m_pause_menu_widget != nullptr)
        {
            m_pause_menu_widget->AddToViewport();
            // This is done in the Pause_Menu blueprint
            // UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(this, m_pause_menu_widget, EMouseLockMode::LockAlways);
            //this->bShowMouseCursor = true;
        }
    }
}

void APIPlayerController::Resume()
{
    if (this->SetPause(false))
    {
        if (m_pause_menu_widget != nullptr)
        {
            // This is done in the Pause_Menu blueprint
            //m_pause_menu_widget->RemoveFromParent();
            //this->bShowMouseCursor = false;
            UWidgetBlueprintLibrary::SetInputMode_GameOnly(this);
        }
    }
}

void APIPlayerController::MoveForward(float value)
{
    if (m_charmed)
    {
        return;
    }

    m_movement_input_y = value;

    if (!m_accept_movement_input || value == 0.0f)
    {
        return;
    }

    if (m_attack_chain_count > 0)
    {
        StopCurrentAttackChain();
    }

    ApplyMovementToAxis(EAxis::X, value);
}

void APIPlayerController::MoveSideways(float value)
{
    if (m_charmed)
    {
        return;
    }

    m_movement_input_x = value;

    if (!m_accept_movement_input || value == 0.0f)
    {
        return;
    }

    if (m_attack_chain_count > 0)
    {
        StopCurrentAttackChain();
    }

    ApplyMovementToAxis(EAxis::Y, value);
}

void APIPlayerController::ApplyMovementToAxis(EAxis::Type axis, float value)
{
    const auto rotation = GetControlRotation();

    GetCharacter()->AddMovementInput(FRotationMatrix{{0.0f, rotation.Yaw, 0.0f}}.GetUnitAxis(axis), value, true);
}

float APIPlayerController::GetMovementInputX()
{
    return m_movement_input_x;
}

float APIPlayerController::GetMovementInputY()
{
    return m_movement_input_y;
}

void APIPlayerController::StartSprinting()
{
    m_sprint_key_down = true;
    GetCharacterMovement()->MaxWalkSpeed = MAX_SPEED_SPRINTING;
}

void APIPlayerController::StopSprinting()
{
    m_sprint_key_down = false;
    GetCharacterMovement()->MaxWalkSpeed = MAX_SPEED_DEFAULT;
}

bool APIPlayerController::IsSprinting()
{
    return m_sprint_key_down;
}

void APIPlayerController::StartDodge()
{
    auto player = GetPlayerCharacter();

    if (!m_accept_movement_input || IsDodging() || GetCharacterMovement()->IsFalling() || m_charmed)
    {
        return;
    }

    if (m_attack_chain_count > 0)
    {
        StopCurrentAttackChain();
    }

    GetCharacterMovement()->Velocity = FVector::ZeroVector;

    // TODO: Snap to front/back/left/right axes
    auto direction = player->GetActorForwardVector();
    auto player_pos = player->GetActorLocation();

    FHitResult hit_result;
    FCollisionQueryParams query_params;

    query_params.AddIgnoredActor(player);
    query_params.bFindInitialOverlaps = true;

    // Uncomment to draw the trace line
    // GetWorld()->DebugDrawTraceTag = "DashTrace";
    // collision_query_params.TraceTag = "DashTrace";

    float dodge_distance = DODGE_ROLL_DISTANCE;

    if (GetWorld()->LineTraceSingleByChannel(
            hit_result, player_pos, player_pos + (direction * DODGE_ROLL_DISTANCE), ECC_Dodging, query_params))
    {
        auto capsule_radius = player->GetCapsuleComponent()->GetScaledCapsuleRadius();
        dodge_distance = hit_result.Distance - capsule_radius;

        if (dodge_distance <= capsule_radius)
        {
            return;
        }
    }

    m_dodge_distance_per_second = dodge_distance / DODGE_ROLL_TIME;
    m_accept_movement_input = false;
    m_dodging = true;

    player->GetCapsuleComponent()->SetCollisionObjectType(ECC_Dodging);
}

void APIPlayerController::StopDodge()
{
    m_accept_movement_input = true;
    m_dodging = false;

    GetPlayerCharacter()->GetCapsuleComponent()->SetCollisionProfileName("PlayerCharacter");
}

bool APIPlayerController::IsDodging()
{
    return m_dodging;
}

void APIPlayerController::PerformLightAttack()
{
    PerformAttack(EPIPlayerAttack::LightMelee);
}

void APIPlayerController::PerformHeavyAttack()
{
    PerformAttack(EPIPlayerAttack::HeavyMelee);
}

void APIPlayerController::PerformLightProjectileAttack()
{
    PerformAttack(EPIPlayerAttack::LightProjectile);
}

void APIPlayerController::PerformHeavyProjectileAttack()
{
    PerformAttack(EPIPlayerAttack::HeavyProjectile);
}

void APIPlayerController::PerformAttack(EPIPlayerAttack attack)
{
    if (IsDodging() || m_charmed || !m_accept_attack_input)
    {
        return;
    }

    if ((attack == EPIPlayerAttack::LightProjectile && GetPlayerCharacter()->GetCurrentProjectileCount() == 0)
        || (attack == EPIPlayerAttack::HeavyProjectile && GetPlayerCharacter()->GetCurrentProjectileCount() < 3))
    {
        return;
    }

    auto max_attack_chain_count = 0u;

    switch (attack)
    {
        case EPIPlayerAttack::LightMelee:
            max_attack_chain_count = m_light_attack_animations.Num();
            break;
        case EPIPlayerAttack::HeavyMelee:
            max_attack_chain_count = m_heavy_attack_animations.Num();
            break;
        case EPIPlayerAttack::LightProjectile:
            max_attack_chain_count = m_light_projectile_attack_animations.Num();
            break;
        case EPIPlayerAttack::HeavyProjectile:
            max_attack_chain_count = 1u;
            break;
        default:
            max_attack_chain_count = 0u;
            break;
    }

    if (m_last_attack != attack)
    {
        if (m_accept_movement_input)
        {
            m_attack_chain_count = 0;
        }
        else
        {
            return;
        }
    }

    if (m_attack_chain_count == 0)
    {
        m_attack_chain_count = 1;
    }
    else
    {
        if (m_attack_chain_count == max_attack_chain_count && m_accept_movement_input)
        {
            m_attack_chain_count = 1;
        }
        else
        {
            if (m_can_increase_attack_chain)
            {
                m_attack_chain_count++;
            }
            else
            {
                return;
            }
        }
    }

    m_accept_movement_input = false;
    m_last_attack = attack;
    m_hit_actors.Empty();

    switch (attack)
    {
        case EPIPlayerAttack::LightMelee:
            GetPlayerCharacter()->PlayAnimMontage(m_light_attack_animations[m_attack_chain_count - 1]);
            break;
        case EPIPlayerAttack::HeavyMelee:
            GetPlayerCharacter()->PlayAnimMontage(m_heavy_attack_animations[m_attack_chain_count - 1]);
            break;
        case EPIPlayerAttack::LightProjectile:
            GetPlayerCharacter()->PlayAnimMontage(m_light_projectile_attack_animations[m_attack_chain_count - 1]);
            break;
        case EPIPlayerAttack::HeavyProjectile:
            GetPlayerCharacter()->PlayAnimMontage(m_heavy_projectile_attack_animation);
            break;
    }
}

void APIPlayerController::FireProjectile(EPIPlayerAttack attack_type, FVector spawn_location)
{
    check(m_light_projectile);
    check(m_heavy_projectile);

    const auto player = GetPlayerCharacter();

    if (player->GetCurrentProjectileCount() == 0)
    {
        return;
    }

    if (attack_type == EPIPlayerAttack::HeavyProjectile && player->GetCurrentProjectileCount() != 3)
    {
        return;
    }

    const auto player_rot = player->GetActorRotation();

    FActorSpawnParameters spawn_params;
    spawn_params.Owner = this;
    spawn_params.Instigator = player;

    AProjectInfernoProjectile* projectile = nullptr;

    switch (attack_type)
    {
        case EPIPlayerAttack::LightProjectile:
            projectile = GetWorld()->SpawnActor<AProjectInfernoProjectile>(
                m_light_projectile, spawn_location, player_rot, spawn_params);
            break;
        case EPIPlayerAttack::HeavyProjectile:
            projectile = GetWorld()->SpawnActor<AProjectInfernoProjectile>(
                m_heavy_projectile, spawn_location, player_rot, spawn_params);
            break;
    }

    if (projectile)
    {
        switch (attack_type)
        {
            case EPIPlayerAttack::LightProjectile:
                player->ConsumeProjectileReserves(1);
                break;
            case EPIPlayerAttack::HeavyProjectile:
                player->ConsumeProjectileReserves(3);
                break;
        }

        projectile->FireInDirection(player_rot.Vector());
    }
}

void APIPlayerController::ResetAttackChain()
{
    m_attack_chain_count = 0;
    m_hit_actors.Empty();
    m_accept_movement_input = true;
}

void APIPlayerController::SetAttackChainCanIncrease(bool can_increase)
{
    m_can_increase_attack_chain = can_increase;
}

void APIPlayerController::StopCurrentAttackChain()
{
    if (m_attack_chain_count == 0)
    {
        return;
    }

    switch (m_last_attack)
    {
        case EPIPlayerAttack::LightMelee:
            GetPlayerCharacter()->StopAnimMontage(m_light_attack_animations[m_attack_chain_count - 1]);
            break;
        case EPIPlayerAttack::HeavyMelee:
            GetPlayerCharacter()->StopAnimMontage(m_heavy_attack_animations[m_attack_chain_count - 1]);
            break;
        case EPIPlayerAttack::LightProjectile:
            GetPlayerCharacter()->StopAnimMontage(m_light_projectile_attack_animations[m_attack_chain_count - 1]);
            break;
        case EPIPlayerAttack::HeavyProjectile:
            GetPlayerCharacter()->StopAnimMontage(m_heavy_projectile_attack_animation);
            break;
    }

    ResetAttackChain();
    GetPlayerCharacter()->GetWeapon()->AttachToComponent(
        GetPlayerCharacter()->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponBack");
}

void APIPlayerController::TurnAtRate(float value)
{
    AddYawInput(value * 45.0f * GetWorld()->GetDeltaSeconds());
}

void APIPlayerController::LookUpAtRate(float value)
{
    AddPitchInput(value * 45.0f * GetWorld()->GetDeltaSeconds());
}

AProjectInfernoPlayerCharacter* APIPlayerController::GetPlayerCharacter()
{
    return Cast<AProjectInfernoPlayerCharacter>(GetCharacter());
}

UCharacterMovementComponent* APIPlayerController::GetCharacterMovement()
{
    return GetPlayerCharacter()->GetCharacterMovement();
}

bool APIPlayerController::IsFalling()
{
    return GetCharacterMovement()->IsFalling();
}

void APIPlayerController::SetMovementInputEnabled(bool enabled)
{
    m_accept_movement_input = true;
}
