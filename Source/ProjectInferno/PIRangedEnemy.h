// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectInfernoPatrolEnemy.h"
#include "PIRangedEnemy.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API APIRangedEnemy : public AProjectInfernoPatrolEnemy
{
    GENERATED_BODY()
public:
    class AProjectInfernoWaypoint* GetNextWaypoint();

    void SetNextWaypoint(AProjectInfernoWaypoint* next_waypoint);
};
