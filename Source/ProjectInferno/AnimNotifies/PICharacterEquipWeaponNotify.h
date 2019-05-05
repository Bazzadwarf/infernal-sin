// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "PICharacter.h"
#include "PICharacterEquipWeaponNotify.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPICharacterEquipWeaponNotify : public UAnimNotify
{
    GENERATED_BODY()
private:
    UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true", DisplayName = "Socket Name"))
    FName m_socket_name = "Weapon Default";

public:
    virtual void Notify(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation) override;
};
