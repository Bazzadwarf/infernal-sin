// Fill out your copyright notice in the Description page of Project Settings.

#include "PIHealthPickUp.h"
#include "Engine.h"

APIHealthPickUp::APIHealthPickUp()
{
    PrimaryActorTick.bCanEverTick = true;
    // Register Events
    OnActorBeginOverlap.AddDynamic(this, &APIHealthPickUp::OnOverlapBegin);
}

// Called when the game starts or when spawned
void APIHealthPickUp::BeginPlay()
{
    Super::BeginPlay();
}

void APIHealthPickUp::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
    // Check if is player exist
    if (OtherActor->ActorHasTag("Player"))
    {
        // Check if the healthcomponent exists on player
        UPIHealthComponent* player_health_component = OtherActor->FindComponentByClass<UPIHealthComponent>();
        if (player_health_component)
        {
            if (m_pick_up_particle_effect != nullptr)
            {
                UGameplayStatics::SpawnEmitterAtLocation(
                    this, m_pick_up_particle_effect, this->ActorToWorld().GetLocation(), FRotator::ZeroRotator, true);
            }

            // Recover health
            player_health_component->GetCurrentHealth();
            player_health_component->RecoverHealth(m_health_points);
            this->Destroy();
        }
    }
}
