#pragma once
#include "GhostState.h"
#include <glm.hpp>
class HouseGhostState :
    public GhostState
{
public:
	void Enter(GhostComponent* ghost) override;
	void Exit(GhostComponent* ghost) override;
	void Update(GhostComponent* ghost, float deltaTime) override;
private:
	float m_Timer{};
	glm::vec2 m_StartPosition;
};

