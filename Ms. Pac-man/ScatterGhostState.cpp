#include "ScatterGhostState.h"
#include "GhostComponent.h"
#include "GridMovementComponent.h"
#include "SpriteComponent.h"
#include "LevelComponent.h"

void ScatterGhostState::Enter(GhostComponent* ghost)
{
	ghost->SetTargetPosition(ghost->GetScatterPosition());
	ghost->ShouldMove(true);
}

void ScatterGhostState::Exit(GhostComponent*ghost)
{
	const auto& gridMovement = ghost->GetGridMovementComponent();
	gridMovement->SetDesiredDirection(-gridMovement->GetDesiredDirection());
}

void ScatterGhostState::Update(GhostComponent* ghost, float )
{
	ghost->SetTargetPosition(ghost->GetScatterPosition());

	auto gridMovementComponent = ghost->GetGridMovementComponent();

	auto levelComponent = gridMovementComponent->GetLevelComponent();

	const glm::vec2& desiredDirection = ghost->GetGridMovementComponent()->GetDesiredDirection();

	const MovementAnim& moveAnim = ghost->GetGridMovementComponent()->GetMovementAnim();

	auto normalSprite = ghost->GetNormalSprite();

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

bool ChaseCondition::Evaluate(GhostComponent* const, float deltaTime)
{
	m_Timer += deltaTime;
	if (m_Timer > 7.f)
	{
		m_Timer = 0.f;
		return true;
	}
	return false;
}
