// Fill out your copyright notice in the Description page of Project Settings.

#include "PIDetectGamepad.h"
#include "SlateBasics.h"

bool UPIDetectGamepad::IsControllerConnected()
{
    auto game = FSlateApplication::Get().GetPlatformApplication();

    if (game.Get() != nullptr && game->IsGamepadAttached())
    {
        return true;
    }

    return false;
}
