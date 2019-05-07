// Fill out your copyright notice in the Description page of Project Settings.

#include "PIEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

constexpr auto DEFAULT_SIGHT_RADIUS = 700.0f;
constexpr auto DEFAULT_SIGHT_LOSS_RADIUS = 850.0f;
constexpr auto DEFAULT_SIGHT_AGE = 5.0f;
constexpr auto DEFAULT_FOV = 90.0f;

APIEnemy::APIEnemy()
{
    m_perception_component = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
    m_sight_config = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Configuration"));

    m_sight_config->SightRadius = DEFAULT_SIGHT_RADIUS;
    m_sight_config->LoseSightRadius = DEFAULT_SIGHT_LOSS_RADIUS;
    m_sight_config->PeripheralVisionAngleDegrees = DEFAULT_FOV;
    m_sight_config->SetMaxAge(DEFAULT_SIGHT_AGE);

    m_sight_config->DetectionByAffiliation.bDetectEnemies = true;
    m_sight_config->DetectionByAffiliation.bDetectNeutrals = true;

    m_perception_component->SetDominantSense(*m_sight_config->GetSenseImplementation());
    m_perception_component->ConfigureSense(*m_sight_config);

    GetHealthComponent()->SetMaxHealth(100.0f);
}

void APIEnemy::BeginPlay()
{
    Super::BeginPlay();
}

void APIEnemy::GetActorEyesViewPoint(FVector& location, FRotator& rotation) const
{
    // NOTE: Head Socket must be X Facing as shown here (Pay attention to the Rotator): https://i.imgur.com/d581M8S.jpg
    location = GetMesh()->GetSocketLocation("HeadSocket");
    rotation = GetMesh()->GetSocketTransform("HeadSocket", RTS_World).Rotator();
}

UAIPerceptionComponent* APIEnemy::GetPerceptionComponent()
{
    return m_perception_component;
}

UBehaviorTree* APIEnemy::GetBehaviourTree()
{
    return m_behaviour_tree;
}

AActor* APIEnemy::GetHomePositionActor()
{
    return m_home_position_actor;
}

void APIEnemy::SetHomePositionActor(AActor* home_position_actor)
{
    m_home_position_actor = home_position_actor;
}

float APIEnemy::GetHealth()
{
    return GetHealthComponent()->GetCurrentHealth();
}

float APIEnemy::GetHealthPercent()
{
    return GetHealth() / GetHealthComponent()->GetMaxHealth();
}
