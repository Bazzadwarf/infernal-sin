// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../PIPlayerController.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "PIPlayerSpawnProjectileAtSocketNotify.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPIPlayerSpawnProjectileAtSocketNotify : public UAnimNotify
{
    GENERATED_BODY()
private:
    UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true", DisplayName = "Projectile Type"))
    EPIPlayerAttack m_projectile_type;

    UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true", DisplayName = "Socket Name"))
    FName m_socket_name;

public:
    virtual void Notify(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation) override;
};
