// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "PIWeapon.generated.h"

UENUM(BlueprintType)
enum class EPIWeaponDataKey : uint8
{
    // Workaround for a bug in UE4 where you can't add a key to the map in the blueprint editor
    // if the default key is in the list of key value pairs
    DefaultKeyDoNotUse,
    MeleeLightDamage,
    MeleeHeavyDamage,
    RangedDamage,
    RangedAmmoRegenRate,
    RangedAmmoMax
};

USTRUCT()
struct FPIWeaponProperties
{
    GENERATED_BODY()
private:
    UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true", DisplayName = "Properties"))
    TMap<EPIWeaponDataKey, float> m_data;

public:
    void SetKeyValue(EPIWeaponDataKey key, float value)
    {
        m_data.Add(key, value);
    }

    TOptional<float> GetKeyValue(EPIWeaponDataKey key) const
    {
        if (auto value = m_data.Find(key))
        {
            return {*value};
        }

        return {};
    }
};

UCLASS()
class PROJECTINFERNO_API APIWeapon : public AActor
{
    GENERATED_BODY()
private:
    UPROPERTY()
    UArrowComponent* m_arrow;

    UPROPERTY(VisibleDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
    UCapsuleComponent* m_collider;

    UPROPERTY(VisibleDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* m_mesh;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon", Meta = (AllowPrivateAccess = "true", DisplayName = "Properties"))
    FPIWeaponProperties m_properties;

    UPROPERTY(EditAnywhere,
              BlueprintReadWrite,
              Category = "Particle Systems",
              Meta = (AllowPrivateAccess = "true", DisplayName = "Weapon Hit"))
    class UParticleSystem* m_hit_particle_system;

public:
    APIWeapon();

    const FPIWeaponProperties& GetWeaponProperties() const;

    void EnableWeaponCollider();
    void DisableWeaponCollider();

    // TODO: Remove single collider object and provide a function to get every collider on the weapon
    //       so we can have multiple colliders per weapon
    UCapsuleComponent* GetCollider();

    USkeletalMeshComponent* GetMesh();

    UParticleSystem* GetParticleSystem()
    {
        return m_hit_particle_system;
    }
};
