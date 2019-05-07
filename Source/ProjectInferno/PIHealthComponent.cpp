// Fill out your copyright notice in the Description page of Project Settings.

#include "PIHealthComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine.h"
#include "PISaveGame.h"
#include "math.h"

// Sets default values for this component's properties
UPIHealthComponent::UPIHealthComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
    // features off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UPIHealthComponent::BeginPlay()
{
    Super::BeginPlay();

    ResetHealth();
    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UPIHealthComponent::TakeDamage);
}

void UPIHealthComponent::TakeDamage(AActor* damaged_actor,
                                    float damage,
                                    const UDamageType* damage_type,
                                    AController* instigated_by,
                                    AActor* damage_causer)
{
    if (damaged_actor == nullptr || m_is_dead || !m_can_damage)
    {
        return;
    }

    m_current_health -= damage;

    OnDamage.ExecuteIfBound({damaged_actor, damage, damage_type, instigated_by, damage_causer});

    if (m_current_health <= 0.0f)
    {
        m_is_dead = true;
        OnDeath.ExecuteIfBound({damaged_actor, damage, damage_type, instigated_by, damage_causer});
    }
}

float UPIHealthComponent::GetMaxHealth()
{
    return m_max_health;
}

float UPIHealthComponent::GetCurrentHealth()
{
    return m_current_health;
}

void UPIHealthComponent::SetMaxHealth(float health, bool reset)
{
    m_max_health = health;

    if (reset)
    {
        ResetHealth();
    }
}

void UPIHealthComponent::ResetHealth()
{
    m_current_health = m_max_health;
    m_is_dead = false;
}

void UPIHealthComponent::RecoverHealth(float healing)
{
    if (m_current_health == m_max_health)
    {
        return;
    }

    m_current_health += healing;

    if (m_current_health > m_max_health)
    {
        m_current_health = m_max_health;
    }
}

bool UPIHealthComponent::IsDead()
{
    return m_is_dead;
}

void UPIHealthComponent::SetIsDead(bool is_dead)
{
    m_is_dead = is_dead;
}

bool UPIHealthComponent::CanDamage()
{
    return m_can_damage;
}

void UPIHealthComponent::SetCanDamage(bool can_damage)
{
    m_can_damage = can_damage;
}

void UPIHealthComponent::RegenerateHealth()
{
    static double DeltaTime = 1 / 30.0;
    static float Healing = 20;

    if (m_current_health == m_max_health)
    {
        return;
    }

    // FMath::Clamp(Healing = Healing * DeltaTime, 0.0f, 30.0f);
    m_current_health += Healing;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Magenta, FString::Printf(TEXT("Plus 20 Health!!")));

    UE_LOG(LogTemp, Warning, TEXT("Plus 20 Health!!"));
    // GEngine->AddOnScreenDebugMessage(
    // -1, 5.f, FColor::Magenta, FString::Printf(TEXT(" Health gen = %s"), healing));

    /* if (m_current_health > m_max_health)
     {
         m_current_health = m_max_health;
     }*/
}

void UPIHealthComponent::LoadHealthDataFromSaveGame(UPISaveGame* save_game)
{
    check(save_game);
    m_current_health = save_game->player_health;
}
