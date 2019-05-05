// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "PIHealthComponent.h"
#include "PIHealthPickUp.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API APIHealthPickUp : public ATriggerBox
{
    GENERATED_BODY()

private:
    // Ammount of Health Points Regenerated
    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Health Regeneration",
              Meta = (AllowPrivateAccess = "true", DisplayName = "HP Regen"))
    float m_health_points = 20.f;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Health Regeneration",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Pick-up Effect"))
    class UParticleSystem* m_pick_up_particle_effect;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Default Constructor
    APIHealthPickUp();

    // Overlap begin function
    UFUNCTION()
    void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
};
