// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectInfernoBoss.h"
#include "ConstructorHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PIBossController.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AProjectInfernoBoss::AProjectInfernoBoss()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    // The character will orient itself to the direction that it is moving in
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 100.0f, 0.0f);

    GetHealthComponent()->SetMaxHealth(1000);
    GetHealthComponent()->OnDeath.BindUFunction(this, "OnDeath");
}

// Called when the game starts or when spawned
void AProjectInfernoBoss::BeginPlay()
{
    Super::BeginPlay();

    // Displays boss HUD UI
    if (m_boss_ui_class != nullptr)
    {
        m_boss_health_widget = CreateWidget<UUserWidget>(GetWorld(), m_boss_ui_class);

        if (m_boss_health_widget != nullptr)
        {
            m_boss_health_widget->AddToViewport();
        }
    }

    if (auto weapon = GetWeapon())
    {
        auto controller = GetController<APIBossController>();

        weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponSocket");

        if (controller)
        {
            weapon->GetCollider()->OnComponentBeginOverlap.AddDynamic(controller, &APIBossController::OnWeaponHit);
        }
    }
}

// Called every frame
void AProjectInfernoBoss::Tick(float delta_time)
{
    Super::Tick(delta_time);
}

// Returns health value
float AProjectInfernoBoss::GetHealth()
{
    return GetHealthComponent()->GetCurrentHealth();
}

// Returns health value out of max health as text
FText AProjectInfernoBoss::GetHealthText()
{
    FString health_string = FString::FromInt(FMath::RoundHalfFromZero(GetHealth()));
    FString max_health_string = FString::FromInt(FMath::RoundHalfFromZero(GetHealthComponent()->GetMaxHealth()));
    FString health_hud = health_string + FString(TEXT(" / ") + max_health_string);
    return FText::FromString(health_hud);
}

// Returns current health as a percent
float AProjectInfernoBoss::GetHealthPercent()
{
    return GetHealth() / GetHealthComponent()->GetMaxHealth();
}

// Recovers health by specified amount up to max health
void AProjectInfernoBoss::RecoverHealth(float healing)
{
    GetHealthComponent()->RecoverHealth(healing);
}

// On boss death
void AProjectInfernoBoss::OnDeath()
{
    GetController<APIBossController>()->OnBossDeath();

    if (m_boss_health_widget != nullptr)
    {
        m_boss_health_widget->RemoveFromViewport();
        m_boss_health_widget = nullptr;
    }
}
