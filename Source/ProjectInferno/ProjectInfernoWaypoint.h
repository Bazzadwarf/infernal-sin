// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/BoxComponent.h"
#include "CoreMinimal.h"
#include "Engine.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "ProjectInfernoWaypoint.generated.h"

UCLASS()
class PROJECTINFERNO_API AProjectInfernoWaypoint : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AProjectInfernoWaypoint();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    USceneComponent* root;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UBoxComponent* box_component;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<AProjectInfernoWaypoint*> connecting_waypoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float m_waypoint_distance_threshold = 2000.0f;

    // Called every frame
    virtual void Tick(float delta_time) override;

    UFUNCTION()
    void PawnEntered(UPrimitiveComponent* overlap_component,
                     AActor* other_actor,
                     UPrimitiveComponent* other_component,
                     int32 other_body_index,
                     bool from_sweep,
                     const FHitResult& sweep_result);

    UFUNCTION()
    FVector GetLocation();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
};
