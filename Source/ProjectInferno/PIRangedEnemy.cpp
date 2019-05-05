// Fill out your copyright notice in the Description page of Project Settings.

#include "PIRangedEnemy.h"

AProjectInfernoWaypoint* APIRangedEnemy::GetNextWaypoint()
{
    return Super::GetNextWaypoint();
}

void APIRangedEnemy::SetNextWaypoint(AProjectInfernoWaypoint* next_waypoint)
{
    Super::SetNextWaypoint(next_waypoint);
}
