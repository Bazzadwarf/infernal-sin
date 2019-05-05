// Fill out your copyright notice in the Description page of Project Settings.

#include "PIMultiTriggerBox.h"

void APIMultiTriggerBox::NotifyActorBeginOverlap(AActor* actor)
{
    Super::NotifyActorBeginOverlap(actor);

    BroadcastEvent(actor, EPITriggerBoxEvent::ActorEntered);
}

void APIMultiTriggerBox::NotifyActorEndOverlap(AActor* actor)
{
    Super::NotifyActorEndOverlap(actor);

    BroadcastEvent(actor, EPITriggerBoxEvent::ActorExited);
}

void APIMultiTriggerBox::BroadcastEvent(AActor* actor, EPITriggerBoxEvent event_type)
{
    if (m_valid_actor_classes.Num() > 0)
    {
        bool valid_class = false;

        for (auto clazz : m_valid_actor_classes)
        {
            if (actor->GetClass()->IsChildOf(clazz))
            {
                valid_class = true;
                break;
            }
        }

        if (!valid_class)
        {
            return;
        }
    }

    UpdateListenerList();

    for (auto listener : m_listeners)
    {
        if (listener.IsStale())
        {
            m_listeners_to_remove.Add(listener);
            continue;
        }

        check(listener->GetClass()->ImplementsInterface(UPIMultiTriggerBoxListener::StaticClass()));
        IPIMultiTriggerBoxListener::Execute_OnMultiTriggerBoxEvent(listener.Get(), this, event_type, actor);
    }

    if (m_destroy_on_trigger)
    {
        this->Destroy();
    }
}

void APIMultiTriggerBox::UpdateListenerList()
{
    if (m_listeners_to_add.Num() > 0)
    {
        m_listeners.Append(m_listeners_to_add);
        m_listeners_to_add.Empty();
    }

    if (m_listeners_to_remove.Num() > 0)
    {
        for (auto listener : m_listeners_to_remove)
        {
            if (m_listeners.Contains(listener))
            {
                m_listeners.Remove(listener);
            }
        }

        m_listeners_to_remove.Empty();
    }
}

void APIMultiTriggerBox::AddListener(AActor* actor)
{
    m_listeners_to_add.Add(actor);
}

void APIMultiTriggerBox::RemoveListener(AActor* actor)
{
    m_listeners_to_remove.Add(actor);
}
