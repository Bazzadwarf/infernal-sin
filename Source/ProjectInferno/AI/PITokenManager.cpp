// Fill out your copyright notice in the Description page of Project Settings.

#include "PITokenManager.h"

UPITokenManager::UPITokenManager()
{
    
}

bool UPITokenManager::RequestToken(EPIEnemyAttackTokenType type)
{
    switch (type)
    {
        case EPIEnemyAttackTokenType::Melee:   
            if (m_currently_used_melee_tokens == MAX_MELEE_TOKENS)
            {
                return false;
            }

            m_currently_used_melee_tokens++;
            return true;
        case EPIEnemyAttackTokenType::Ranged:
            if (m_currently_used_ranged_tokens == MAX_RANGED_TOKENS)
            {
                return false;
            }

            m_currently_used_ranged_tokens++;
            return true;
    }

    return false;
}

void UPITokenManager::ReleaseToken(EPIEnemyAttackTokenType type)
{
    switch (type)
    {
        case EPIEnemyAttackTokenType::Melee:
            if (m_currently_used_melee_tokens != 0)
            {
                m_currently_used_melee_tokens--;
            }
            break;
        case EPIEnemyAttackTokenType::Ranged:
            if (m_currently_used_ranged_tokens == MAX_RANGED_TOKENS)
            {
                m_currently_used_ranged_tokens--;
            }
            break;
    }
}

void UPITokenManager::Reset()
{
    m_currently_used_melee_tokens = 0;
    m_currently_used_ranged_tokens = 0;
}
