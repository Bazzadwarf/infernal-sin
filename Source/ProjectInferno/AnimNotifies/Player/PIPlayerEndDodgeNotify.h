// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "PIPlayerEndDodgeNotify.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPIPlayerEndDodgeNotify : public UAnimNotify
{
    GENERATED_BODY()
public:
    virtual void Notify(USkeletalMeshComponent* mesh_component, UAnimSequenceBase* animation) override;
};
