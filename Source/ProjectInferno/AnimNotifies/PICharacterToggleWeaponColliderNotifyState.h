// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "PICharacter.h"
#include "PICharacterToggleWeaponColliderNotifyState.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPICharacterToggleWeaponColliderNotifyState : public UAnimNotifyState
{
    GENERATED_BODY()
public:
    virtual void
    NotifyBegin(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation, float total_duration) override;

    virtual void
    NotifyTick(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation, float frame_delta_time) override;

    virtual void NotifyEnd(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation) override;
};
