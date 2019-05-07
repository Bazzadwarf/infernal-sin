// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BehaviorTree.h"
#include "CoreMinimal.h"
#include "PICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "PIEnemy.generated.h"

/**
 *
 */
UCLASS() class PROJECTINFERNO_API APIEnemy : public APICharacter
{
    GENERATED_BODY()
private:
    UPROPERTY(VisibleDefaultsOnly)
    UAIPerceptionComponent* m_perception_component;

    UPROPERTY(VisibleDefaultsOnly)
    UAISenseConfig_Sight* m_sight_config;

    UPROPERTY(EditDefaultsOnly, Category = "AI", Meta = (AllowPrivateAccess = "true", DisplayName = "Behaviour Tree"))
    UBehaviorTree* m_behaviour_tree;

    UPROPERTY(EditAnywhere, Category = "AI", Meta = (AllowPrivateAccess = "true", DisplayName = "Home Position Actor"))
    AActor* m_home_position_actor = nullptr;

public:
    APIEnemy();

    virtual void BeginPlay() override;

    virtual void GetActorEyesViewPoint(FVector& location, FRotator& rotation) const override;

    UFUNCTION(BlueprintCallable)
    UAIPerceptionComponent* GetPerceptionComponent();

    UBehaviorTree* GetBehaviourTree();

    UFUNCTION()
    AActor* GetHomePositionActor();

    UFUNCTION()
    void SetHomePositionActor(AActor* home_position_actor);

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealth();

    UFUNCTION(BlueprintCallable, Category = "Health")
    float GetHealthPercent();
};
