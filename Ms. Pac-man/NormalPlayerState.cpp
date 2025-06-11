#include "NormalPlayerState.h"
#include "PlayerComponent.h"
#include "LevelComponent.h"
#include "MovementInputComponent.h"
#include "DyingPlayerState.h"
#include "SpriteComponent.h"
#include "GridMovementComponent.h"

void NormalPlayerState::Enter(PlayerComponent* player)
{
    if (!player->GetSpriteComponent())return;
    player->GetSpriteComponent()->PlaySprite();
    auto gridComponent = player->GetGridMovementComponent();
    if (gridComponent)
    {
        gridComponent->ShouldMove(true);
        gridComponent->SetSpeed(GetMovementSpeed());
    }
}

void NormalPlayerState::Exit(PlayerComponent* player)
{
    if (!player->GetSpriteComponent())return;
    player->GetSpriteComponent()->StopSprite();
    auto gridComponent = player->GetGridMovementComponent();
    if (gridComponent)
    {
        gridComponent->ShouldMove(false);
    }
}

void NormalPlayerState::Update(PlayerComponent* player, float deltaTime)
{
    auto levelComponent = player->GetLevelComponent();

    auto gridComponent = player->GetGridMovementComponent();

    if (!gridComponent) return;

    if (!levelComponent) return;

    auto pos = gridComponent->GetAccumulatedPosition();

    if (pos.x < 0.f)
    {
        pos.x = levelComponent->WIDTH - 1;
        gridComponent->SetAccumulatedPosition(pos);
        player->GetMovementInput()->RemoveInput();
    }
    if (pos.x > levelComponent->WIDTH - 1)
    {
        pos.x = 0.1f;
        gridComponent->SetAccumulatedPosition(pos);
        player->GetMovementInput()->RemoveInput();
    }

    gridComponent->SetDesiredDirection(player->GetMovementInput()->GetMovementInput());
    gridComponent->UpdatePosition(deltaTime);
    player->ProcessMovement(deltaTime);

    player->CheckDotCollection();
    gridComponent->SetDesiredDirection(glm::vec2(0,0));
}