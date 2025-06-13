#include "ChaseGhostState.h"
#include "GhostComponent.h"
#include "GridMovementComponent.h"
#include "SpriteComponent.h"
#include "GameObject.h"
#include "PlayerComponent.h"
#include "LevelComponent.h"
#include <cmath>
#define _USE_MATH_DEFINES
#include <math.h>

void ChaseGhostState::Enter(GhostComponent* ghost)
{
	m_pPlayerComponent = ghost->GetPacman()->GetComponent<PlayerComponent>();
}

void ChaseGhostState::Exit(GhostComponent* ghost)
{
	const auto& gridMovement = ghost->GetGridMovementComponent();
	gridMovement->SetDesiredDirection(-gridMovement->GetDesiredDirection());
}

void ChaseGhostState::Update(GhostComponent* ghost, float)
{
	if (m_pPlayerComponent == nullptr) return;

	auto gridMovementComponent = ghost->GetGridMovementComponent();

	auto levelComponent = gridMovementComponent->GetLevelComponent();

	auto pos = gridMovementComponent->GetAccumulatedPosition();

	if (pos.x < 0.f)
	{
		pos.x = levelComponent->WIDTH - 1;
		gridMovementComponent->SetAccumulatedPosition(pos);
	}
	if (pos.x > levelComponent->WIDTH - 1)
	{
		pos.x = 0.1f;
		gridMovementComponent->SetAccumulatedPosition(pos);
	}

	const float TILE_WIDTH = ghost->GetGridMovementComponent()->GetLevelComponent()->TILE_WIDTH;

	switch (ghost->GetGhostType())
	{
	case GhostType::Red:
		ghost->SetTargetPosition(ghost->GetPacman()->GetWorldPosition());
		break;
	case GhostType::Pink:
	{
		glm::vec3 playerDesiredDir = { m_pPlayerComponent->GetFacingDirection(),0};

		ghost->SetTargetPosition(ghost->GetPacman()->GetWorldPosition() + playerDesiredDir * 4.f* TILE_WIDTH);
		break;
	}
	case GhostType::Cyan:
	{
		glm::vec2 playerDesiredDir = m_pPlayerComponent->GetFacingDirection();

		glm::vec2 playerPos = glm::vec2(ghost->GetPacman()->GetWorldPosition());
		glm::vec2 targetPos = playerPos + playerDesiredDir * 2.0f* TILE_WIDTH;

		glm::vec2 blinkyPos = glm::vec2(ghost->GetBlinky()->GetWorldPosition());

		glm::vec2 vector = blinkyPos - targetPos;
		glm::vec2 finalTarget = blinkyPos + vector;

		ghost->SetTargetPosition(finalTarget);
		break;
	}
	case GhostType::Orange:

		glm::vec2 playerPos = glm::vec2(ghost->GetPacman()->GetWorldPosition());
		glm::vec2 ghostPos = glm::vec2(ghost->GetOwner()->GetWorldPosition());
		if (glm::distance(playerPos, ghostPos) >= ghost->GetGridMovementComponent()->GetLevelComponent()->TILE_WIDTH * 8.f)
		{
			ghost->SetTargetPosition(playerPos);
		}
		else {
			ghost->SetTargetPosition(ghost->GetScatterPosition());
		}

		break;
	}

	const glm::vec2& desiredDirection = ghost->GetGridMovementComponent()->GetDesiredDirection();

	const MovementAnim& moveAnim = ghost->GetGridMovementComponent()->GetMovementAnim();

	auto normalSprite = ghost->GetNormalSprite();

	if (moveAnim.moveX)
	{
		if (desiredDirection.x < 0.f)
		{
			normalSprite->SetSpriteY(1);
		}
		else if (desiredDirection.x > 0.f)
		{
			normalSprite->SetSpriteY(0);
		}
		normalSprite->PlaySprite();
	}
	if (moveAnim.moveY)
	{
		if (desiredDirection.y < 0.f)
		{
			normalSprite->SetSpriteY(2);
		}
		else if (desiredDirection.y > 0.f)
		{
			normalSprite->SetSpriteY(3);
		}
		normalSprite->PlaySprite();
	}
	if (moveAnim.mustStop)
	{
		normalSprite->StopSprite();
	}
}
