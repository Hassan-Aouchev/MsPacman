#include "NormalPlayerState.h"
#include "PlayerComponent.h"
#include "LevelComponent.h"
#include "MovementInputComponent.h"
#include "DyingPlayerState.h"
#include "SpriteComponent.h"

void NormalPlayerState::Enter(PlayerComponent* player)
{
    player->SetSpeed(GetMovementSpeed());
    if (!player->GetSpriteComponent())return;
    player->GetSpriteComponent()->PlaySprite();
}

void NormalPlayerState::Exit(PlayerComponent* player)
{
    if (!player->GetSpriteComponent())return;
    player->GetSpriteComponent()->StopSprite();
}

void NormalPlayerState::Update(PlayerComponent* player, float deltaTime)
{
    auto levelComponent = player->GetLevelComponent();

    if (!levelComponent) return;

    auto pos = player->GetAccumulatedPosition();

    if (pos.x < 0.f)
    {
        pos.x = levelComponent->WIDTH - 1;
        player->SetAccumulatedPosition(pos);
        player->GetMovementInput()->RemoveInput();
    }
    if (pos.x > levelComponent->WIDTH - 1)
    {
        pos.x = 0.1f;
        player->SetAccumulatedPosition(pos);
        player->GetMovementInput()->RemoveInput();
    }

    player->ProcessMovement(deltaTime);

    player->CheckDotCollection();
}