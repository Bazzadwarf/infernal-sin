// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectInfernoGameMode.h"
#include "ConstructorHelpers.h"
#include "Engine.h"
#include "PIGameInstance.h"
#include "PIPlayerController.h"
#include "ProjectInfernoPlayerCharacter.h"

AProjectInfernoGameMode::AProjectInfernoGameMode()
{
    static ConstructorHelpers::FClassFinder<AProjectInfernoPlayerCharacter> player_class{
        TEXT("Blueprint'/Game/Blueprints/Player/PlayerCharacter'")};

    if (player_class.Succeeded())
    {
        DefaultPawnClass = player_class.Class;
    }

    static ConstructorHelpers::FClassFinder<APIPlayerController> player_controller_class{
        TEXT("Blueprint'/Game/Blueprints/Player/PlayerController_BP'")};

    if (player_controller_class.Succeeded())
    {
        PlayerControllerClass = player_controller_class.Class;
    }
}

AActor* AProjectInfernoGameMode::ChoosePlayerStart_Implementation(AController* player)
{
    auto save_game = Cast<UPIGameInstance>(GetWorld()->GetGameInstance())->GetSaveGameObject();

    // Save game will be null if launched via PIE. There are probably other places we need to check for null as well.
    if (save_game)
    {
        auto actor_tag = FName{*save_game->checkpoint_actor_tag};

        for (TActorIterator<APlayerStart> iterator{GetWorld()}; iterator; ++iterator)
        {
            if (iterator->PlayerStartTag == actor_tag)
            {
                return *iterator;
            }
        }
    }

    return Super::ChoosePlayerStart_Implementation(player);
}
