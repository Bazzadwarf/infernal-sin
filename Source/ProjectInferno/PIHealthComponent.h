// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "PIHealthComponent.generated.h"

USTRUCT()
struct FPIDamageInfo
{
    GENERATED_BODY()

    UPROPERTY()
    AActor* damaged_actor;

    UPROPERTY()
    float damage;

    UPROPERTY()
    const class UDamageType* damage_type;

    UPROPERTY()
    class AController* instigated_by;

    UPROPERTY()
    AActor* damage_causer;
};

DECLARE_DELEGATE_OneParam(FOnDeathDelegate, FPIDamageInfo);
DECLARE_DELEGATE_OneParam(FOnDamagedDelegate, FPIDamageInfo);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTINFERNO_API UPIHealthComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    FOnDeathDelegate OnDeath;
    FOnDamagedDelegate OnDamaged;

private:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float m_max_health;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    float m_current_health;

    bool m_is_dead = false;

    bool m_can_damage = true;

public:
    // Sets default values for this component's properties
    UPIHealthComponent();

    UFUNCTION()
    void TakeDamage(AActor* damaged_actor,
                    float damage,
                    const UDamageType* damage_type,
                    AController* intigated_by,
                    AActor* damage_causer);

    UFUNCTION()
    float GetMaxHealth();

    UFUNCTION()
    float GetCurrentHealth();

    UFUNCTION()
    void SetMaxHealth(float health, bool reset = false);

    UFUNCTION()
    void ResetHealth();

    UFUNCTION()
    void RecoverHealth(float healing);

    UFUNCTION()
    bool IsDead();

    UFUNCTION()
    void SetIsDead(bool is_dead);

    UFUNCTION()
    bool CanDamage();

    UFUNCTION()
    void SetCanDamage(bool can_damage);

    UFUNCTION()
    virtual void RegenerateHealth();

    UFUNCTION()
    void LoadHealthDataFromSaveGame(class UPISaveGame* save_game);

protected:
    // Called when the game starts
    virtual void BeginPlay() override;
};
