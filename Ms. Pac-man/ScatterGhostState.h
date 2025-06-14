#pragma once
#include "GhostState.h"


class ChaseCondition :public GhostCondition
{
public:
    bool Evaluate(GhostComponent* const ghost, float deltaTime) override;
private:
    float m_Timer{};
};

class ScatterGhostState :
    public GhostState
{
public:
    void Enter(GhostComponent* ghost)override;
    void Exit(GhostComponent* ghost)override;
    void Update(GhostComponent* ghost, float deltaTime)override;
private:
};

