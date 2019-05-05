// Fill out your copyright notice in the Description page of Project Settings.

#include "PICharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APICharacter::APICharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    m_health_component = CreateDefaultSubobject<UPIHealthComponent>(TEXT("Health Component"));
}

void APICharacter::BeginPlay()
{
    Super::BeginPlay();

    if (m_default_weapon_class != nullptr)
    {
        m_weapon = GetWorld()->SpawnActor<APIWeapon>(m_default_weapon_class);

        check(m_weapon);

        if (m_weapon != nullptr)
        {
            m_weapon->SetOwner(this);
            // TODO: Attach weapon to unequipped socket
            m_weapon->DisableWeaponCollider();
        }
    }
}

UPIHealthComponent* APICharacter::GetHealthComponent()
{
    return m_health_component;
}

APIWeapon* APICharacter::GetWeapon()
{
    return m_weapon;
}
