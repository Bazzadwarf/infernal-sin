// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectInfernoProjectile.h"
#include "PIEnemySpawner.h"
#include "ProjectInfernoPlayerCharacter.h"

// Sets default values
AProjectInfernoProjectile::AProjectInfernoProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    m_collision_component = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    m_collision_component->OnComponentHit.AddDynamic(this, &AProjectInfernoProjectile::OnHit);
    m_collision_component->InitSphereRadius(10.0f);
    RootComponent = m_collision_component;

    m_projectile_movement_component
        = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("m_projectile_movement_component"));
    m_projectile_movement_component->SetUpdatedComponent(m_collision_component);
    m_projectile_movement_component->InitialSpeed = m_starting_speed;
    m_projectile_movement_component->MaxSpeed = 3000.0f;
    m_projectile_movement_component->bRotationFollowsVelocity = true;
    m_projectile_movement_component->bShouldBounce = false;

    InitialLifeSpan = m_initial_life_span;
}

// Called when the game starts or when spawned
void AProjectInfernoProjectile::BeginPlay()
{
    Super::BeginPlay();

    if (this->ActorHasTag(TEXT("Homing")))
    {
        FindPlayer();
    }

    m_internal_location = this->GetActorLocation();
    m_world_location = m_internal_location;

    m_sine_wave_length = m_initial_life_span / m_sine_wave_per_life;
}

// Called every frame
void AProjectInfernoProjectile::Tick(float delta_time)
{
    Super::Tick(delta_time);

    m_life_time += delta_time;

    m_internal_velocity = m_acceleration * delta_time + m_internal_velocity;
    // m_internal_location = this->GetActorLocation();
    m_internal_location = m_internal_location + m_internal_velocity;

    m_world_location = m_internal_location;

    if (m_target != nullptr && this->ActorHasTag(TEXT("Homing")))
    {
        auto direction = (m_target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        direction += m_target->GetVelocity().GetSafeNormal() * direction.Size() / 200.0f;
        m_projectile_movement_component->Velocity = direction * m_homing_speed;
    }

    if (m_sine_wave)
    {
        sine = FMath::Sin(m_life_time / m_sine_wave_length);

        if (m_sine_horizontal)
        {
            m_world_location += this->GetActorRightVector() * sine * m_sine_magnitude;
        }

        if (m_sine_verticle)
        {
            m_world_location += this->GetActorUpVector() * sine * m_sine_magnitude;
        }

        if (m_sine_depth)
        {
            m_world_location += this->GetActorForwardVector() * sine * m_sine_magnitude;
        }
    }
	else if(m_cos_wave)
	{
		cos = FMath::Cos(m_life_time / m_cos_wave_length);
		
		if (m_cos_horizontal)
        {
            m_world_location += this->GetActorRightVector() * cos * m_cos_magnitude;
        }

        if (m_cos_verticle)
        {
            m_world_location += this->GetActorUpVector() * cos * m_cos_magnitude;
        }

        if (m_cos_depth)
        {
            m_world_location += this->GetActorForwardVector() * cos * m_cos_magnitude;
        }
	}	

    this->SetActorLocation(m_world_location);
}

void AProjectInfernoProjectile::FireInDirection(const FVector& shoot_direction)
{
    m_internal_direction = shoot_direction;
    m_internal_velocity = shoot_direction * m_starting_speed;
}

void AProjectInfernoProjectile::OnHit(UPrimitiveComponent* hit_component,
                                      AActor* other_actor,
                                      UPrimitiveComponent* other_component,
                                      FVector normal_impulse,
                                      const FHitResult& hit)
{
    if (other_actor != this && other_actor && Instigator)
    {
        if (m_on_destory_particle_effect != nullptr)
        {
            UGameplayStatics::SpawnEmitterAtLocation(
                this, m_on_destory_particle_effect, this->ActorToWorld().GetLocation(), FRotator::ZeroRotator, true);
        }

        if (m_on_destroy_sound != nullptr)
        {
            UGameplayStatics::SpawnSoundAtLocation(this,
                                                   m_on_destroy_sound,
                                                   this->ActorToWorld().GetLocation(),
                                                   FRotator::ZeroRotator,
                                                   1.0f,
                                                   1.0f,
                                                   0.0f,
                                                   nullptr,
                                                   nullptr,
                                                   true);
        }

        if (this->ActorHasTag("Enemy_Projectile"))
        {
            if (!other_actor->ActorHasTag("Enemy_Projectile"))
            {
                if (other_actor->ActorHasTag("Player"))
                {
                    other_actor->TakeDamage(m_damage, FPointDamageEvent{}, Instigator->GetController(), this);
                    if (this->ActorHasTag("Charm"))
                    {
                        auto player = Cast<AProjectInfernoPlayerCharacter>(other_actor);
                        player->Charm();
                    }
                    this->Destroy();
                }
                else if (other_actor->ActorHasTag("Wall"))
                {
                    this->Destroy();
                }

                if (!this->ActorHasTag("Bouncy"))
                {
                    this->Destroy();
                }
            }
        }
        else if (this->ActorHasTag("Portal_Projectile"))
        {
            if (!other_actor->ActorHasTag("Player_Projectile"))
            {
                auto position = this->GetActorLocation();
                auto rotation = FRotator(0.0f, 0.0f, 0.0f);
                auto world = GetWorld();

                if (world)
                {
                    FActorSpawnParameters spawn_params;
                    spawn_params.Owner = this;
                    spawn_params.Instigator = Instigator;

                    world->SpawnActor<APIEnemySpawner>(m_portal, position, rotation, spawn_params);
                }
            }

            this->Destroy();
        }
        else if (this->ActorHasTag("Player_Projectile"))
        {
            if (other_actor->ActorHasTag("Enemy_Projectile"))
            {
                other_actor->Destroy();
            }
            else if (other_actor->ActorHasTag("Enemy"))
            {
                other_actor->TakeDamage(m_damage, FPointDamageEvent{}, Instigator->GetController(), this);
            }

            this->Destroy();
        }
    }
    else
    {
        this->Destroy();
    }
}

void AProjectInfernoProjectile::FindPlayer()
{
    auto world = GetWorld();

    if (world)
    {
        for (TActorIterator<AProjectInfernoPlayerCharacter> actor_iterator(world); actor_iterator; ++actor_iterator)
        {
            class AProjectInfernoPlayerCharacter* player = *actor_iterator;

            if (player != nullptr)
            {
                if (player->ActorHasTag(TEXT("Player")))
                {
                    m_target = player;
                }
            }
        }
    }
}

float AProjectInfernoProjectile::GetHomingSpeed()
{
    return m_homing_speed;
}

void AProjectInfernoProjectile::SetHomingSpeed(float speed)
{
    m_homing_speed = speed;
}
