// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "PICharacter.h"
#include "PICharacterSetMovementModeNotify.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPICharacterSetMovementModeNotify : public UAnimNotify
{
    GENERATED_BODY()
private:
    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true", DisplayName = "Movement Mode"))
    TEnumAsByte<EMovementMode> m_movement_mode;

public:
    virtual void Notify(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation) override;
};
