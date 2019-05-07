// Fill out your copyright notice in the Description page of Project Settings.

#include "PIEnemyController.h"
#include "../PIGameInstance.h"
#include "../ProjectInfernoPlayerCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "PIEnemyMelee.h"

APIEnemyController::APIEnemyController(const FObjectInitializer& object_initializer)
    : Super(object_initializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>(TEXT("PathFollowingComponent")))
{
    m_blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
    m_behaviour_tree = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("Behaviour Tree"));
}

void APIEnemyController::Possess(APawn* pawn)
{
    Super::Possess(pawn);

    auto enemy = Cast<APIEnemy>(pawn);
    check(enemy);

    SetPerceptionComponent(*enemy->GetPerceptionComponent());
    enemy->GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &APIEnemyController::OnPerceptionUpdated);

    enemy->GetCharacterMovement()->MaxWalkSpeed = m_max_walking_speed;

    if (auto behaviour_tree = enemy->GetBehaviourTree())
    {
        m_blackboard->InitializeBlackboard(*behaviour_tree->BlackboardAsset);
        m_behaviour_tree->StartTree(*behaviour_tree);
    }

    // Required to feed accelleration value in to the movement blend space (No way to set this without creating a custom
    // movement component class so just set these values in the enemy blueprint: https://i.imgur.com/RqzHYZG.png
    ensure(enemy->GetMovementComponent()->UseAccelerationForPathFollowing());

    enemy->GetHealthComponent()->OnDamaged.BindUFunction(this, "OnDamaged");
    enemy->GetHealthComponent()->OnDeath.BindUFunction(this, "OnDeath");

    if (auto melee_enemy = Cast<APIEnemyMelee>(enemy))
    {
        melee_enemy->GetLeftHandCollider()->OnComponentBeginOverlap.AddDynamic(this, &APIEnemyController::OnMeleeHit);
        melee_enemy->GetRightHandCollider()->OnComponentBeginOverlap.AddDynamic(this, &APIEnemyController::OnMeleeHit);
    }
}

void APIEnemyController::Tick(float delta_time)
{
    Super::Tick(delta_time);
}

void APIEnemyController::OnPerceptionUpdated(const TArray<AActor*>& actors)
{
    for (auto actor : actors)
    {
        if (!actor->GetClass()->IsChildOf(AProjectInfernoPlayerCharacter::StaticClass()))
        {
            continue;
        }

        FActorPerceptionBlueprintInfo perception_info;
        GetPerceptionComponent()->GetActorsPerception(actor, perception_info);

        for (auto& stimulus : perception_info.LastSensedStimuli)
        {
            if (!stimulus.IsValid() || stimulus.IsExpired())
            {
                continue;
            }

            // We don't need to check the sense ID since we're only using the sight sense

            // If the stimulus is active i.e. the player is still within sight then track it
            if (stimulus.IsActive())
            {
                m_player_reference = Cast<AProjectInfernoPlayerCharacter>(actor);
                SetFocus(m_player_reference, EAIFocusPriority::Gameplay);
            }
            else
            {
                m_player_reference = nullptr;
                ReleaseAttackToken();
                SetFocus(nullptr, EAIFocusPriority::Gameplay);
            }
        }
    }
}

void APIEnemyController::StartAttack()
{
    m_is_attacking = true;
    m_hit_actors.Empty();
}

void APIEnemyController::StopAttack()
{
    m_is_attacking = false;
    ReleaseAttackToken();
}

bool APIEnemyController::IsAttacking()
{
    return m_is_attacking;
}

bool APIEnemyController::TryAcquireAttackToken()
{
    if (m_has_attack_token)
    {
        return true;
    }

    if (auto game_instance = Cast<UPIGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
    {
        m_has_attack_token = game_instance->GetAITokenManager()->RequestToken(m_attack_token_type);
    }

    return m_has_attack_token;
}

bool APIEnemyController::HasAttackToken()
{
    return m_has_attack_token;
}

void APIEnemyController::ReleaseAttackToken()
{
    if (m_has_attack_token)
    {
        auto game_instance = Cast<UPIGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
        game_instance->GetAITokenManager()->ReleaseToken(m_attack_token_type);
        m_has_attack_token = false;
    }
}

APIEnemy* APIEnemyController::GetEnemy()
{
    return Cast<APIEnemy>(GetCharacter());
}

AProjectInfernoPlayerCharacter* APIEnemyController::GetPlayerReference()
{
    return m_player_reference;
}

void APIEnemyController::SetIsRunning(bool is_running)
{
    m_is_running = is_running;
}

bool APIEnemyController::IsRunning()
{
    return m_is_running;
}

void APIEnemyController::StartStun()
{
    m_is_stunned = true;
    ReleaseAttackToken();

    if (auto melee_enemy = Cast<APIEnemyMelee>(GetEnemy()))
    {
        melee_enemy->DisableHandColliders();
    }
}

void APIEnemyController::EndStun()
{
    m_is_stunned = false;
}

bool APIEnemyController::IsStunned()
{
    return m_is_stunned;
}

void APIEnemyController::OnDamaged(FPIDamageInfo info)
{
    if (GetEnemy()->GetHealthComponent()->GetCurrentHealth() > 0)
    {
        StartStun();
        GetEnemy()->PlayAnimMontage(m_knockback_animation);
    }
}

void APIEnemyController::OnDeath(FPIDamageInfo info)
{
    if (auto melee_enemy = Cast<APIEnemyMelee>(GetEnemy()))
    {
        melee_enemy->GetLeftHandCollider()->OnComponentBeginOverlap.RemoveDynamic(this,
                                                                                  &APIEnemyController::OnMeleeHit);
        melee_enemy->GetRightHandCollider()->OnComponentBeginOverlap.RemoveDynamic(this,
                                                                                   &APIEnemyController::OnMeleeHit);
    }

    ReleaseAttackToken();

    // Do ragdoll
    auto enemy = GetEnemy();

    enemy->DetachFromControllerPendingDestroy();
    enemy->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    enemy->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

    enemy->GetMesh()->SetCollisionProfileName("Ragdoll");
    enemy->SetActorEnableCollision(true);

    enemy->GetMesh()->SetAllBodiesSimulatePhysics(true);
    enemy->GetMesh()->SetSimulatePhysics(true);
    enemy->GetMesh()->WakeAllRigidBodies();
    enemy->GetMesh()->bBlendPhysics = true;

    if (auto character_comp = enemy->GetCharacterMovement())
    {
        character_comp->StopMovementImmediately();
        character_comp->DisableMovement();
        character_comp->SetComponentTickEnabled(false);
    }

    enemy->SetLifeSpan(10.0f);
}

const TArray<UAnimMontage*>& APIEnemyController::GetAttackAnimationsArray()
{
    return m_attack_animations;
}

void APIEnemyController::OnMeleeHit(UPrimitiveComponent* hit_component,
                                    AActor* other_actor,
                                    UPrimitiveComponent* other_component,
                                    int32 other_body_index,
                                    bool b_from_sweep,
                                    const FHitResult& sweep_result)
{
    if (!other_actor->GetClass()->IsChildOf(AProjectInfernoPlayerCharacter::StaticClass())
        || m_hit_actors.Contains(other_actor))
    {
        return;
    }

    m_hit_actors.Add(other_actor);

    other_actor->TakeDamage(Cast<APIEnemyMelee>(GetEnemy())->GetAttackDamage(), FPointDamageEvent{}, this, GetEnemy());
}
