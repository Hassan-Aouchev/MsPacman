#include "RetryGhostState.h"
#include "GhostComponent.h"
#include "GameObject.h"
#include "GridMovementComponent.h"

void RetryGhostState::Enter(GhostComponent* ghost)
{
	ghost->GetOwner()->SetLocalPosition(glm::vec2(-1000, -1000));
	ghost->ShouldMove(false);
}

void RetryGhostState::Exit(GhostComponent* ghost)
{
	ghost->GetGridMovementComponent()->SetAccumulatedPosition(glm::vec2(13,11));
	ghost->ShouldMove(true);
}
