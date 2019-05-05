// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PICharacter.h"
#include "TimerManager.h"
#include "ProjectInfernoBasicEnemy.generated.h"

UCLASS()
class PROJECTINFERNO_API AProjectInfernoBasicEnemy : public APICharacter
{
    GENERATED_BODY()

private:
    UPROPERTY(VisibleDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
    class UPawnSensingComponent* m_pawn_sensing_component;

    UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (AllowPrivateAccess = "true", DisplayName = "Behaviour Tree"))
    class UBehaviorTree* m_behaviour_tree;

    // Timer Handle to keep track of our timer
    FTimerHandle m_timer_handle;

public:
    // Sets default values for this character's properties
    AProjectInfernoBasicEnemy();

    UFUNCTION()
    class UPawnSensingComponent* GetPawnSensingComponent()
    {
        return m_pawn_sensing_component;
    }

    UFUNCTION()
    class UBehaviorTree* GetBehaviorTree()
    {
        return m_behaviour_tree;
    }

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnSeePlayer(APawn* pawn);

    UFUNCTION()
    void OnLosePlayer();
};
