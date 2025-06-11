#pragma once
#include <State.h>

class PlayerComponent;

class PlayerCondition : public dae::Condition<PlayerComponent>
{
};

class PlayerState : public dae::State<PlayerComponent>
{
public:
	virtual bool CanMove() const = 0;
	virtual bool CanEatDots() const = 0;
	virtual bool IsVulnerable() const = 0;
	virtual float GetMovementSpeed() const = 0;
};