#pragma once
#include "GhostState.h"

class ScatterGhostState :
    public GhostState
{
public:
    void Enter(GhostComponent*)override{}
    void Exit(GhostComponent*)override{}
    void Update(GhostComponent*,float)override{}
};

