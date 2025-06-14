#pragma once
#include "PlayerState.h"
class Texture2DComponent;

class DeathCondition : public PlayerCondition
{
public:
	bool Evaluate(PlayerComponent* player, float deltaTime) override;
};

class RetryCondition : public PlayerCondition
{
public:
	bool Evaluate(PlayerComponent* player, float deltaTime) override;
};

class DyingPlayerState : public PlayerState
{
public:

	void Enter(PlayerComponent* player) override;
	void Exit(PlayerComponent* player) override;
	void Update(PlayerComponent* player, float deltaTime) override;

	bool CanMove() const override { return true; }
	bool CanEatDots() const override { return true; }
	bool IsVulnerable() const override { return true; }
	float GetMovementSpeed() const override { return 0.0f; }

private:
	float m_currentRotation = 0.0f;
	float m_rotationSpeed = 360.0f;

	Texture2DComponent* m_TextureComp;
};

