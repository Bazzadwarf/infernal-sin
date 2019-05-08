// Fill out your copyright notice in the Description page of Project Settings.

#include "PIEnemyMelee.h"

APIEnemyMelee::APIEnemyMelee()
{
    m_left_hand_collider = CreateDefaultSubobject<UBoxComponent>(TEXT("LeftHandCollider"));
    m_right_hand_collider = CreateDefaultSubobject<UBoxComponent>(TEXT("RightHandCollider"));

    m_left_hand_collider->SetCollisionProfileName("Weapon");
    m_right_hand_collider->SetCollisionProfileName("Weapon");

    auto mesh = GetMesh();

    if (mesh)
    {
        m_left_hand_collider->SetupAttachment(mesh, "LeftHandSocket");
        m_right_hand_collider->SetupAttachment(mesh, "RightHandSocket");
    }
}

void APIEnemyMelee::BeginPlay()
{
    Super::BeginPlay();

    DisableHandColliders();
}

void APIEnemyMelee::EnableHandColliders()
{
    m_left_hand_collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    m_right_hand_collider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APIEnemyMelee::DisableHandColliders()
{
    m_left_hand_collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_right_hand_collider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UBoxComponent* APIEnemyMelee::GetLeftHandCollider()
{
    return m_left_hand_collider;
}

UBoxComponent* APIEnemyMelee::GetRightHandCollider()
{
    return m_right_hand_collider;
}

float APIEnemyMelee::GetAttackDamage()
{
    return m_attack_damage;
}
