#pragma once
#include "GhostState.h"

class PlayerComponent;

class ScatterCondition :public GhostCondition
{
public:
	bool Evaluate(GhostComponent* const) override { return false; }
};

class ChaseGhostState : public GhostState
{
public:
	void Enter(GhostComponent* ghost) override;
	void Exit(GhostComponent* ghost) override;
	void Update(GhostComponent* ghost, float deltaTime) override;
private:
	PlayerComponent* m_pPlayerComponent = nullptr;
};

