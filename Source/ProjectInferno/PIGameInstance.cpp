// Fill out your copyright notice in the Description page of Project Settings.

#include "PIGameInstance.h"
#include "Core/Public/Misc/Paths.h"
#include "Engine.h"
#include "FileManager.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectInfernoPlayerCharacter.h"

bool UPIGameInstance::GetIsTutorialEnabled()
{
    return is_tutorial_enabled;
}

void UPIGameInstance::SetIsTutorialEnabled(bool value)
{
    is_tutorial_enabled = value;
}

UPISaveGame* UPIGameInstance::LoadGameFromSlot(FString save_slot_name)
{
    if (auto save = Cast<UPISaveGame>(UGameplayStatics::LoadGameFromSlot(save_slot_name, 0)))
    {
        m_save_game_object = save;
        return m_save_game_object;
    }

    return nullptr;
}

bool UPIGameInstance::SaveGame()
{
    if (m_save_game_object == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("SaveGame called without a loaded save game object."))
        return false;
    }

    // Update Player Data
    auto player = Cast<AProjectInfernoPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

    m_save_game_object->player_health = player->GetHealth();
    m_save_game_object->player_ammo_count = player->GetCurrentProjectileCount();
    m_save_game_object->player_ammo_percentage
        = player->GetProjectileRegenProgress(m_save_game_object->player_ammo_count + 1);

    // Update current level name
    m_save_game_object->level_name = GetWorld()->GetMapName();
    m_save_game_object->level_name.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

    return UGameplayStatics::SaveGameToSlot(m_save_game_object, m_save_game_object->player_name, 0);
}

void UPIGameInstance::UpdatePlayerStartActorForSaveGame(APlayerStart* actor)
{
    check(m_save_game_object);
    check(actor);

    m_save_game_object->checkpoint_actor_tag = actor->PlayerStartTag.ToString();
    SaveGame();
}

UPISaveGame* UPIGameInstance::GetSaveGameObject()
{
    return m_save_game_object;
}

// TODO: This should probably fail in case of the save file already existing (currently handled in blueprint)
UPISaveGame* UPIGameInstance::CreateNewSaveGame(FString player_name)
{
    if (auto save = Cast<UPISaveGame>(UGameplayStatics::CreateSaveGameObject(UPISaveGame::StaticClass())))
    {
        save->player_name = player_name;

        if (UGameplayStatics::SaveGameToSlot(save, player_name, 0))
        {
            m_save_game_object = save;
            return m_save_game_object;
        }
    }

    return nullptr;
}

TArray<FPISaveFileInfo> UPIGameInstance::GetAllSaveFilesFromDisk()
{
    TArray<FPISaveFileInfo> files;

    auto& file_manager = IFileManager::Get();
    auto saved_games_folder = FString{FPaths::ProjectSavedDir()} + "SaveGames/";

    TArray<FString> file_names;
    file_manager.FindFiles(file_names, *saved_games_folder, TEXT("*.sav"));

    for (auto file_name : file_names)
    {
        file_name.RemoveFromEnd(".sav");
        auto save_game_object = Cast<UPISaveGame>(UGameplayStatics::LoadGameFromSlot(file_name, 0));

        if (save_game_object)
        {
            files.Add({file_name,
                       save_game_object->player_name,
                       save_game_object->level_name,
                       file_manager.GetTimeStamp(*(saved_games_folder + file_name + ".sav")).ToString()});
        }
    }

    return files;
}
