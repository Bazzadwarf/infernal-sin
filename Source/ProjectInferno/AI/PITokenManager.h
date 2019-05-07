// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PITokenManager.generated.h"

UENUM(BlueprintType)
enum class EPIEnemyAttackTokenType : uint8
{
    Melee,
    Ranged
};

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPITokenManager : public UObject
{
    GENERATED_BODY()
private:
    static constexpr auto MAX_MELEE_TOKENS = 2u;
    static constexpr auto MAX_RANGED_TOKENS = 3u;

    unsigned int m_currently_used_melee_tokens = 0;

    unsigned int m_currently_used_ranged_tokens = 0;

public:
    UPITokenManager();

    bool RequestToken(EPIEnemyAttackTokenType type);

    void ReleaseToken(EPIEnemyAttackTokenType type);

    void Reset();
};
