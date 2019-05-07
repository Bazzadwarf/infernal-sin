// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectInfernoWaypoint.h"
#include "ProjectInfernoPatrolEnemy.h"

// Sets default values
AProjectInfernoWaypoint::AProjectInfernoWaypoint()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    root = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
    SetRootComponent(root);

    box_component = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Volume"));
    box_component->SetupAttachment(GetRootComponent());
    box_component->OnComponentBeginOverlap.AddDynamic(this, &AProjectInfernoWaypoint::PawnEntered);
}

// Called when the game starts or when spawned
void AProjectInfernoWaypoint::BeginPlay()
{
    Super::BeginPlay();

    for (TActorIterator<AProjectInfernoWaypoint> waypoints(GetWorld()); waypoints; ++waypoints)
    {
        if (*waypoints != this)
        {
            if (this->GetDistanceTo(*waypoints) <= m_waypoint_distance_threshold)
            {
                connecting_waypoints.Add(*waypoints);
            }
        }
    }
}

// Called every frame
void AProjectInfernoWaypoint::Tick(float delta_time)
{
    Super::Tick(delta_time);

    for (auto waypoint : connecting_waypoints)
    {
        DrawDebugLine(GetWorld(), GetActorLocation(), waypoint->GetActorLocation(), FColor::Red, true, -1.0f, 0, 2.0f);
    }
}

void AProjectInfernoWaypoint::PawnEntered(UPrimitiveComponent* overlap_component,
                                          AActor* other_actor,
                                          UPrimitiveComponent* other_component,
                                          int32 other_body_index,
                                          bool from_sweep,
                                          const FHitResult& sweep_result)
{
    if (other_actor != nullptr)
    {
        auto patrol_enemy = Cast<AProjectInfernoPatrolEnemy>(other_actor);

        // If the cast succeeded
        if (patrol_enemy != nullptr)
        {
            if (connecting_waypoints.Num() > 0)
            {
                patrol_enemy->SetNextWaypoint(connecting_waypoints[rand() % connecting_waypoints.Num()]);
            }
            else
            {
                patrol_enemy->SetNextWaypoint(this);
            }
        }
    }
}

FVector AProjectInfernoWaypoint::GetLocation()
{
    return GetActorLocation();
}
