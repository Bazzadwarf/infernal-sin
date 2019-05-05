// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PICharacter.h"
#include "PIHealthComponent.h"
#include "UserWidget.h"
#include "ProjectInfernoBoss.generated.h"

UCLASS()
class PROJECTINFERNO_API AProjectInfernoBoss : public APICharacter
{
    GENERATED_BODY()

private:
    UPROPERTY(EditAnywhere, Category = "Boss", Meta = (BlueprintProtected = "true", DisplayName = "Boss HUD"))
    TSubclassOf<class UUserWidget> m_boss_ui_class;

    UPROPERTY()
    class UUserWidget* m_boss_health_widget;

    UPROPERTY(EditAnywhere, Category = "Boss", Meta = (AllowPrivateAccess = "true", DisplayName = "Name"))
    FText m_boss_name;

public:
    AProjectInfernoBoss();

    virtual void Tick(float delta_time) override;

    UFUNCTION()
    float GetHealth();

    UFUNCTION(BlueprintPure, Category = "Health")
    FText GetHealthText();

    UFUNCTION(BlueprintPure, Category = "Health")
    float GetHealthPercent();

    UFUNCTION()
    void RecoverHealth(float healing);

    UFUNCTION()
    virtual void OnDeath();

protected:
    virtual void BeginPlay() override;
};
