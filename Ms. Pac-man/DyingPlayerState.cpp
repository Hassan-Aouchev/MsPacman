#include "DyingPlayerState.h"
#include <SpriteComponent.h>
#include "PlayerComponent.h"
#include "Texture2DComponent.h"
#include "SpriteComponent.h"

void DyingPlayerState::Enter(PlayerComponent* player)
{
	m_TextureComp = player->GetSpriteComponent()->GetTextureComp();
	if (m_TextureComp != nullptr)
	{
		m_currentRotation = 0.0f;
		m_TextureComp->ClearRotationCenter();
	}
	player->GetSubject()->NotifyObservers(player->GetOwner(), "ActorDied");
}

void DyingPlayerState::Exit(PlayerComponent* )
{
	if (!m_TextureComp) return;

	m_TextureComp->SetRotation(0.0f);
	m_TextureComp = nullptr;
}

void DyingPlayerState::Update(PlayerComponent* , float deltaTime)
{
    if (m_TextureComp == nullptr) return;

    m_currentRotation += m_rotationSpeed * deltaTime;

    while (m_currentRotation >= 360.0f)
        m_currentRotation -= 360.0f;
    while (m_currentRotation < 0.0f)
        m_currentRotation += 360.0f;

    m_TextureComp->SetRotation(m_currentRotation);
}

bool DeathCondition::Evaluate(PlayerComponent* player)
{
	return !player->IsAlive();
}
