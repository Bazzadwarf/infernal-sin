// Fill out your copyright notice in the Description page of Project Settings.

#include "PIWeapon.h"

APIWeapon::APIWeapon()
{
    PrimaryActorTick.bCanEverTick = true;

    m_arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
    RootComponent = m_arrow;

    m_collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Weapon Collider"));
    m_mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon Mesh"));

    m_collider->SetupAttachment(m_arrow);
    m_mesh->SetupAttachment(m_arrow);

    m_collider->SetCollisionProfileName("Weapon");
    m_mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APIWeapon::EnableWeaponCollider()
{
    m_collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APIWeapon::DisableWeaponCollider()
{
    m_collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

const FPIWeaponProperties& APIWeapon::GetWeaponProperties() const
{
    return m_properties;
}

UCapsuleComponent* APIWeapon::GetCollider()
{
    return m_collider;
}

USkeletalMeshComponent* APIWeapon::GetMesh()
{
    return m_mesh;
}
