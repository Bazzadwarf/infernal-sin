// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AI/PITokenManager.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PISaveGame.h"
#include "PIGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FPISaveFileInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", Meta = (DisplayName = "Save Slot Name"))
    FString save_slot_name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", Meta = (DisplayName = "Player Name"))
    FString player_name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", Meta = (DisplayName = "Current Level"))
    FString current_level;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info", Meta = (DisplayName = "Date Time Saved"))
    FString date_time_saved;
};

/**
 *
 */
UCLASS()
class PROJECTINFERNO_API UPIGameInstance : public UGameInstance
{
    GENERATED_BODY()
private:
    // It would be nice if this and all of the other settings in the Options menu were saved
    bool is_tutorial_enabled = true;

    UPROPERTY()
    UPISaveGame* m_save_game_object = nullptr;

    UPROPERTY()
    UPITokenManager* m_token_manager = nullptr;

public:
    UFUNCTION(BlueprintCallable)
    bool GetIsTutorialEnabled();

    UFUNCTION(BlueprintCallable)
    void SetIsTutorialEnabled(bool value);

    UFUNCTION(BlueprintCallable)
    UPISaveGame* LoadGameFromSlot(FString save_slot_name);

    UFUNCTION(BlueprintCallable)
    bool SaveGame(bool reset_defaults);

    UFUNCTION(BlueprintCallable)
    void UpdatePlayerStartActorForSaveGame(APlayerStart* actor);

    UFUNCTION(BlueprintCallable)
    UPISaveGame* GetSaveGameObject();

    UFUNCTION(BlueprintCallable)
    UPISaveGame* CreateNewSaveGame(FString player_name);

    UFUNCTION(BlueprintCallable)
    TArray<FPISaveFileInfo> GetAllSaveFilesFromDisk();

    UFUNCTION(BlueprintCallable)
    UPITokenManager* GetAITokenManager();
};
