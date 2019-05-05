// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "PIPlayerToggleMovementInputNotify.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPIPlayerToggleMovementInputNotify : public UAnimNotify
{
    GENERATED_BODY()
private:
    UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true", DisplayName = "Movement Input Enabled?"))
    bool m_movement_input_enabled = false;

public:
    virtual void Notify(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation) override;
};
