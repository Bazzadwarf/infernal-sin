// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PIHealthComponent.h"
#include "PIWeapon.h"
#include "PICharacter.generated.h"

UCLASS(Abstract)
class PROJECTINFERNO_API APICharacter : public ACharacter
{
    GENERATED_BODY()
private:
    UPROPERTY(EditDefaultsOnly, Meta = (AllowPrivateAccess = "true", DisplayName = "Default Weapon"))
    TSubclassOf<APIWeapon> m_default_weapon_class;

    UPROPERTY(VisibleDefaultsOnly, Meta = (AllowPrivateAccess = "true"))
    UPIHealthComponent* m_health_component = nullptr;

    UPROPERTY()
    APIWeapon* m_weapon = nullptr;

public:
    APICharacter();

    UFUNCTION(BlueprintCallable)
    UPIHealthComponent* GetHealthComponent();

    UFUNCTION(BlueprintCallable)
    APIWeapon* GetWeapon();
protected:
    virtual void BeginPlay() override;
};
