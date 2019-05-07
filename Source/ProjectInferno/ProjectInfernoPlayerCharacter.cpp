// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectInfernoPlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "PIPlayerController.h"
#include "ProjectInfernoProjectile.h"
#include "UserWidget.h"
#include "Navigation/CrowdManager.h"
#include <algorithm>

AProjectInfernoPlayerCharacter::AProjectInfernoPlayerCharacter()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need
    // it.
    PrimaryActorTick.bCanEverTick = true;

    // Don't rotate when the controller rotates, only let that affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Setup character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator{0.0f, 10000.0f, 0.0f};
    GetCharacterMovement()->JumpZVelocity = 600.0f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Setup the camera boom
    m_camera_boom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    m_camera_boom->SetupAttachment(RootComponent);
    m_camera_boom->bDoCollisionTest = true;
    m_camera_boom->bUsePawnControlRotation = true;
    m_camera_boom->TargetArmLength = 700.0f;
    m_camera_boom->bEnableCameraLag = true;
    m_camera_boom->CameraLagSpeed = 2.5f;
    m_camera_boom->SocketOffset = {0, 0.0f, 220.0f};

    // Setup the camera component
    m_camera_component = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
    m_camera_component->SetupAttachment(m_camera_boom, USpringArmComponent::SocketName);
    m_camera_component->bUsePawnControlRotation = false;
    m_camera_component->SetRelativeRotation({-10.0f, 0.0f, 0.0f});

    GetHealthComponent()->SetMaxHealth(100);
    GetHealthComponent()->OnDeath.BindUFunction(this, "OnDeath");
}

// Called when the game starts or when spawned
void AProjectInfernoPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    m_current_stamina = MAX_STAMINA;

    // Create death screen widget
    if (m_death_screen_class != nullptr)
    {
        m_death_screen_widget = CreateWidget<UUserWidget>(GetWorld(), m_death_screen_class);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("You need to add the death screen widget to the player character!"));
    }

    if (auto weapon = GetWeapon())
    {
        weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "WeaponBack");

        auto controller = GetController<APIPlayerController>();

        weapon->GetCollider()->OnComponentBeginOverlap.RemoveDynamic(GetController<APIPlayerController>(),
                                                                     &APIPlayerController::OnWeaponHit);

        weapon->GetCollider()->OnComponentBeginOverlap.AddDynamic(GetController<APIPlayerController>(),
                                                                  &APIPlayerController::OnWeaponHit);
    }

    if (auto crowd_manager = UCrowdManager::GetCurrent(this))
    {
        crowd_manager->RegisterAgent(this);
    }
}

void AProjectInfernoPlayerCharacter::Tick(float delta_time)
{
    Super::Tick(delta_time);
}

void AProjectInfernoPlayerCharacter::Charm()
{
    Cast<APIPlayerController>(GetController())->Charm();
}

float AProjectInfernoPlayerCharacter::GetHealth()
{
    return GetHealthComponent()->GetCurrentHealth();
}

FText AProjectInfernoPlayerCharacter::GetHealthText()
{
    FString health_string = FString::FromInt(FMath::RoundHalfFromZero(GetHealth()));
    FString max_health_string = FString::FromInt(FMath::RoundHalfFromZero(GetHealthComponent()->GetMaxHealth()));
    FString health_hud = health_string + FString(TEXT(" / ") + max_health_string);
    return FText::FromString(health_hud);
}

float AProjectInfernoPlayerCharacter::GetHealthPercent()
{
    return GetHealth() / GetHealthComponent()->GetMaxHealth();
}

float AProjectInfernoPlayerCharacter::GetStamina()
{
    return m_current_stamina;
}

FText AProjectInfernoPlayerCharacter::GetStaminaText()
{
    FString stamina_string = FString::FromInt(FMath::RoundHalfFromZero(m_current_stamina));
    FString max_stamina_string = FString::FromInt(FMath::RoundHalfFromZero(MAX_STAMINA));
    FString stamina_hud = stamina_string + FString(TEXT(" / ") + max_stamina_string);
    return FText::FromString(stamina_hud);
}

float AProjectInfernoPlayerCharacter::GetStaminaPercent()
{
    return m_current_stamina / MAX_STAMINA;
}

void AProjectInfernoPlayerCharacter::UseStamina(float stamina_use)
{
    if (m_current_stamina - stamina_use >= 0)
    {
        m_current_stamina -= stamina_use;
    }
}

void AProjectInfernoPlayerCharacter::RecoverStamina(float stamina_recovery)
{
    if (m_current_stamina != MAX_STAMINA)
    {
        m_current_stamina += stamina_recovery;

        if (m_current_stamina > MAX_STAMINA)
        {
            m_current_stamina = MAX_STAMINA;
        }
    }
}

int AProjectInfernoPlayerCharacter::GetCurrentProjectileCount()
{
    return m_current_projectile_count;
}

float AProjectInfernoPlayerCharacter::GetProjectileRegenProgress(int projectile)
{
    if (m_current_projectile_count >= projectile)
    {
        return 1.0f;
    }

    if (m_current_projectile_count == projectile - 1)
    {
        return m_projectile_regen_progress;
    }

    return 0.0f;
}

void AProjectInfernoPlayerCharacter::ConsumeProjectileReserves(int amount)
{
    m_current_projectile_count = std::max(0, m_current_projectile_count - amount);
}

void AProjectInfernoPlayerCharacter::RegenerateProjectile(float amount)
{
    if (m_current_projectile_count == MAX_PROJECTILE_COUNT)
    {
        m_projectile_regen_progress = 0.0f;
        return;
    }

    // TODO: There seems to be a bug where it is possble to overcharge progress?

    if ((m_projectile_regen_progress += amount) >= 1.0f)
    {
        m_current_projectile_count++;

        if (m_current_projectile_count == MAX_PROJECTILE_COUNT)
        {
            m_projectile_regen_progress = 0.0f;
        }
        else
        {
            m_projectile_regen_progress -= 1.0f;
        }
    }
}

void AProjectInfernoPlayerCharacter::SetCameraLag(float lag)
{
    m_camera_boom->CameraLagSpeed = lag;
}

void AProjectInfernoPlayerCharacter::OnDeath()
{
    // This is now done in the splash screen
    // UGameplayStatics::OpenLevel(GetWorld(), "MainMenuLevel");

    float random_sound = FMath::RandRange(0, m_death_sounds.Num() - 1);

    if (m_death_sounds.Num() > 0)
    {
        //TODO: Isn't there meant to be a brief period of invincibility after each hit? There needs to be a check based on that so that this very nice lady doesn't scream like 100x a second
        UGameplayStatics::SpawnSoundAtLocation(this,
                                    m_death_sounds[random_sound],
                                    this->GetActorLocation(),
                                    FRotator::ZeroRotator,
                                    1.0f,
                                    1.0f,
                                    0.0f,
                                    nullptr,
                                    nullptr,
                                    true);
    }

    if (m_death_screen_widget != nullptr)
    {
        m_death_screen_widget->AddToViewport();
    }

    // auto world = GetWorld();
    // auto controller = this->GetController();
    // GetWeapon()->Destroy();
    // this->Destroy();
    // world->GetAuthGameMode()->RestartPlayer(controller);
}

bool AProjectInfernoPlayerCharacter::CanBeSeenFrom(const FVector& observer_location,
                                                   FVector& out_seen_location,
                                                   int32& number_of_los_checks_performed,
                                                   float& out_sight_strength,
                                                   const AActor* ignore_actor) const
{
    FVector origin;
    FVector box_extent;

    auto capsule_component = GetCapsuleComponent();

    if (capsule_component)
    {
        float capsule_half_height = capsule_component->GetScaledCapsuleHalfHeight();

        // Check every Z coord on the player capsule component to see if it is visible
        for (float i = capsule_half_height * 2; i > 0; i -= 10)
        {
            bool result = CalculateHit(observer_location,
                                       out_seen_location,
                                       number_of_los_checks_performed,
                                       out_sight_strength,
                                       ignore_actor,
                                       (GetActorLocation() - FVector{0.0f, 0.0f, capsule_half_height})
                                           + FVector{0.0f, 0.0f, i});

            if (result)
            {
                return result;
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("PlayerCharacter is missing CapsuleComponent"));
    }

    return false;
}

bool AProjectInfernoPlayerCharacter::CalculateHit(const FVector& observer_location,
                                                  FVector& out_seen_location,
                                                  int32& number_of_los_checks_performed,
                                                  float& out_sight_strength,
                                                  const AActor* ignore_actor,
                                                  FVector hit_location) const
{
    FHitResult hit_result;

    // Check if the lower part of the player can be seen
    const bool hit = GetWorld()->LineTraceSingleByObjectType(
        hit_result,
        observer_location,
        hit_location,
        FCollisionObjectQueryParams(ECC_TO_BITFIELD(ECC_WorldStatic) | ECC_TO_BITFIELD(ECC_WorldDynamic)),
        FCollisionQueryParams(FName(TEXT("TestPawnLineOfSight")), true, ignore_actor));

    number_of_los_checks_performed++;

    if (!hit || (hit_result.Actor.IsValid() && hit_result.Actor->IsOwnedBy(this)))
    {
        out_seen_location = hit_location;
        out_sight_strength = 1;
        return true;
    }

    // Cannot be seen
    out_sight_strength = 0;
    return false;
}
