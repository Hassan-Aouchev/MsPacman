#pragma once
#include "GhostState.h"
class RetryGhostState :
    public GhostState
{
public:
    void Update(GhostComponent*, float) {};
    void Enter(GhostComponent* ghost)override;
    void Exit(GhostComponent* ghost)override;
};

