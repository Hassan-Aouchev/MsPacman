#include "HouseGhostState.h"
#include "GhostComponent.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "GridMovementComponent.h"
#include "LevelComponent.h"

void HouseGhostState::Enter(GhostComponent* ghost)
{
	m_Timer = 0.f;
	ghost->ShouldMove(false);
	m_StartPosition = ghost->GetOwner()->GetTransform().GetLocalPosition();

	switch (ghost->GetGhostType()) {
	case GhostType::Red: ghost->SetMaxWaitTime(0.f); break;
	case GhostType::Pink:  ghost->SetMaxWaitTime(0.f); break;
	case GhostType::Cyan:   ghost->SetMaxWaitTime(7.f); break;
	case GhostType::Orange:  ghost->SetMaxWaitTime(17.f); break;
	}
}

void HouseGhostState::Exit(GhostComponent* ghost)
{
	ghost->ShouldMove(true);
}

void HouseGhostState::Update(GhostComponent* ghost, float deltaTime)
{
	const auto levelComponent = ghost->GetGridMovementComponent()->GetLevelComponent();

	ghost->GetOwner()->SetLocalPosition(m_StartPosition+sin(m_Timer)* levelComponent->TILE_WIDTH);
	m_Timer += deltaTime;

	auto normalSprite = ghost->GetNormalSprite();

	if (m_StartPosition.y < ghost->GetOwner()->GetTransform().GetLocalPosition().y)
	{
		normalSprite->SetSpriteY(2);
	}
	else {
		normalSprite->SetSpriteY(3);
	}
}
