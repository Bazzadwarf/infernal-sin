// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TriggerBox.h"
#include "PIMultiTriggerBox.generated.h"

UENUM()
enum class EPITriggerBoxEvent : uint8
{
    ActorEntered,
    ActorExited
};

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API APIMultiTriggerBox : public ATriggerBox
{
    GENERATED_BODY()
private:
    UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true", DisplayName = "Listeners"))
    TArray<TWeakObjectPtr<AActor>> m_listeners;

    TArray<TWeakObjectPtr<AActor>> m_listeners_to_add;
    TArray<TWeakObjectPtr<AActor>> m_listeners_to_remove;

    UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true", DisplayName = "Destroy on Trigger"))
    bool m_destroy_on_trigger = false;

    UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = "true", DisplayName = "Actor Filter"))
    TArray<TSubclassOf<AActor>> m_valid_actor_classes;

public:
    virtual void NotifyActorBeginOverlap(AActor* actor) override;

    virtual void NotifyActorEndOverlap(AActor* actor) override;

    UFUNCTION(BlueprintCallable)
    void AddListener(AActor* actor);

    UFUNCTION(BlueprintCallable)
    void RemoveListener(AActor* actor);

private:
    void UpdateListenerList();

    void BroadcastEvent(AActor* actor, EPITriggerBoxEvent event_type);
};

/**
 *
 */
UINTERFACE()
class PROJECTINFERNO_API UPIMultiTriggerBoxListener : public UInterface
{
    GENERATED_BODY()
};

class PROJECTINFERNO_API IPIMultiTriggerBoxListener
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintImplementableEvent)
    void OnMultiTriggerBoxEvent(class APIMultiTriggerBox* trigger_box, EPITriggerBoxEvent event_type, AActor* actor);
};
