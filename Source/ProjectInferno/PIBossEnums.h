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
    LightAttack,
    HeavyAttack,
    OverheadAttack,
    SweepAttack,
    RamAttack,
    LeftAttack,
    RightAttack,
    StompAttack,
    AxeStompAttack,
    HomingProjectile,
    SpawnerProjectile,
    ConeProjectiles,
    ReversedConeProjectiles,
    AOEProjectiles,
    SpiralProjectiles,
    BurstProjectiles,
    Dead
};

UENUM(BlueprintType)
enum class PlayerSide : uint8
{
    Front,
    Left,
    Right,
    Back
};
