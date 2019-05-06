// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class BossPhases : uint8
{
    Idle_Phase,
    Phase_1,
    Phase_2,
    Phase_3,
    Phase_4
};

UENUM(BlueprintType)
enum class BossStates : uint8
{
    Idle,
    Idle2,
    Following,
    Attacking,
    Moving,
    Melee1,
    Melee2,
    Melee3,
    Melee4,
    Melee5,
    LeftAttack,
    RightAttack,
    StompAttack,
    AxeStompAttack,
    HomingProjectile,
    SpawnerProjectile,
    ConeProjectiles,
    ReversedConeProjectiles,
    SwipeLeftToRight,
    SwipeRightToLeft,
    AOEProjectiles,
    WaveAOEProjectiles,
    SpiralProjectiles,
    BurstProjectiles,
    Dead,
    Flying,
    Resting,
    Landing,
    TakingOff,
    FlyingProjectile1,
    FlyingProjectile2,
    FlyingProjectile3
};

UENUM(BlueprintType)
enum class PlayerSide : uint8
{
    Front,
    Left,
    Right
};
