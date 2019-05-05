// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PIDetectGamepad.generated.h"

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPIDetectGamepad : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, meta = (DisplayName = "IsControllerConnected"), Category = "System Information")
    static bool IsControllerConnected();
};
