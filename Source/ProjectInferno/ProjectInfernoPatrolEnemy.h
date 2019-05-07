// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PICharacter.h"
#include "ProjectInfernoWaypoint.h"
#include "ProjectInfernoPatrolEnemy.generated.h"

UCLASS()
class PROJECTINFERNO_API AProjectInfernoPatrolEnemy : public APICharacter
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class AProjectInfernoWaypoint* m_next_waypoint;

public:
    // Sets default values for this character's properties
    AProjectInfernoPatrolEnemy();

    virtual class AProjectInfernoWaypoint* GetNextWaypoint();

    virtual void SetNextWaypoint(AProjectInfernoWaypoint* next_waypoint);

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealth();

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent();

    UFUNCTION()
    void OnDeath();

protected:
    virtual void BeginPlay() override;
};
