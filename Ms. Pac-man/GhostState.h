#pragma once
#include <State.h>
class GhostComponent;

class GhostCondition : public dae::Condition<GhostComponent>
{
};

class GhostState : public dae::State<GhostComponent>
{
};

