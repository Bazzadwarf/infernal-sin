// Fill out your copyright notice in the Description page of Project Settings.

#include "PIHeavyAdd.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PIHeavyAddController.h"

// Sets default values
APIHeavyAdd::APIHeavyAdd()
{
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    m_left_hand_collider = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollider"));
    m_right_hand_collider = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollider"));
    m_body_collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyCollider"));

    m_left_hand_collider->SetCollisionProfileName("Weapon");
    m_right_hand_collider->SetCollisionProfileName("Weapon");
    m_body_collider->SetCollisionProfileName("Weapon");

    auto mesh = GetMesh();

    if (mesh)
    {
        m_left_hand_collider->SetupAttachment(mesh, "LeftHandSocket");
        m_right_hand_collider->SetupAttachment(mesh, "RightHandSocket");
        m_body_collider->SetupAttachment(mesh, "BodySocket");
    }

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 100.0f, 0.0f);

    GetHealthComponent()->SetMaxHealth(500);
    GetHealthComponent()->OnDeath.BindUFunction(this, "OnDeath");
    GetHealthComponent()->OnDamaged.BindUFunction(this, "OnDamaged");
}

void APIHeavyAdd::BeginPlay()
{
    Super::BeginPlay();

    auto controller = GetController<APIHeavyAddController>();
}

void APIHeavyAdd::Tick(float delta_time)
{
    Super::Tick(delta_time);

    if (m_dead)
    {
        m_dead_timer += delta_time;
        this->GetMovementComponent()->AddInputVector(FVector{0, 0, 2}, true);
        if (m_dead_timer >= 6)
        {
            this->Destroy();
        }
    }
}

void APIHeavyAdd::EnableLeftHandColliders()
{
    m_left_hand_collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APIHeavyAdd::DisableLeftHandColliders()
{
    m_left_hand_collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APIHeavyAdd::EnableRightHandColliders()
{
    m_right_hand_collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APIHeavyAdd::DisableRightHandColliders()
{
    m_right_hand_collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APIHeavyAdd::EnableBodyCollider()
{
    m_body_collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APIHeavyAdd::DisableBodyCollider()
{
    m_body_collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UBoxComponent* APIHeavyAdd::GetLeftHandCollider()
{
    return m_left_hand_collider;
}

UBoxComponent* APIHeavyAdd::GetRightHandCollider()
{
    return m_right_hand_collider;
}

UBoxComponent* APIHeavyAdd::GetBodyCollider()
{
    return m_body_collider;
}

// Returns health value
float APIHeavyAdd::GetHealth()
{
    return GetHealthComponent()->GetCurrentHealth();
}

// Returns health value out of max health as text
FText APIHeavyAdd::GetHealthText()
{
    FString health_string = FString::FromInt(FMath::RoundHalfFromZero(GetHealth()));
    FString max_health_string = FString::FromInt(FMath::RoundHalfFromZero(GetHealthComponent()->GetMaxHealth()));
    FString health_hud = health_string + FString(TEXT(" / ") + max_health_string);
    return FText::FromString(health_hud);
}

// Returns current health as a percent
float APIHeavyAdd::GetHealthPercent()
{
    return GetHealth() / GetHealthComponent()->GetMaxHealth();
}

// Recovers health by specified amount up to max health
void APIHeavyAdd::RecoverHealth(float healing)
{
    GetHealthComponent()->RecoverHealth(healing);
}

// On boss death
void APIHeavyAdd::OnDeath(FPIDamageInfo info)
{
    GetController<APIHeavyAddController>()->OnDeath(info);
}

void APIHeavyAdd::Dead()
{
    m_dead = true;
}

void APIHeavyAdd::OnDamaged(FPIDamageInfo info)
{
    GetController<APIHeavyAddController>()->OnDamaged(info);
}
