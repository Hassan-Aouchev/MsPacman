#pragma once
#include "PlayerState.h"
class NormalPlayerState : public PlayerState
{
public:
	void Enter(PlayerComponent* player) override;
	void Exit(PlayerComponent* player) override;
	void Update(PlayerComponent* player, float deltaTime) override;

	bool CanMove() const override { return true; }
	bool CanEatDots() const override { return true; }
	bool IsVulnerable() const override { return true; }
	float GetMovementSpeed() const override { return 5.0f; }
};

