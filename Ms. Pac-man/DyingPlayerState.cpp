#include "DyingPlayerState.h"
#include <SpriteComponent.h>
#include "PlayerComponent.h"
#include "Texture2DComponent.h"
#include "SpriteComponent.h"
#include "LevelComponent.h"
#include "GridMovementComponent.h"

void DyingPlayerState::Enter(PlayerComponent* player)
{
	m_TextureComp = player->GetSpriteComponent()->GetTextureComp();
	if (m_TextureComp != nullptr)
	{
		m_currentRotation = 0.0f;
		m_TextureComp->ClearRotationCenter();
	}
	player->GetSubject()->NotifyObservers(player->GetOwner(), "ActorDied");
	m_currentRotation = 0.f;
}

void DyingPlayerState::Exit(PlayerComponent* player)
{
	if (!m_TextureComp) return;

	m_TextureComp->SetRotation(0.0f);
	m_TextureComp = nullptr;

	player->GetGridMovementComponent()->SetAccumulatedPosition(glm::vec2(13, 23));

	if (player->GetLives() > 0)
	{
		player->GetSubject()->NotifyObservers(player->GetOwner(), "ActorRevived");
	}
	else {
		player->GetSubject()->NotifyObservers(player->GetOwner(), "RestartLevel");
	}
}

void DyingPlayerState::Update(PlayerComponent* player, float deltaTime)
{
    if (m_TextureComp == nullptr) return;

    m_currentRotation += m_rotationSpeed * deltaTime;

	if (m_currentRotation >= 720.f)
	{
		player->Retry();
	}

    m_TextureComp->SetRotation(m_currentRotation);
}

bool DeathCondition::Evaluate(PlayerComponent* player, float)
{
	return player->IsDead();
}

bool RetryCondition::Evaluate(PlayerComponent* player, float )
{
	return !player->IsDead();
}
