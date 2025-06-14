#include "GhostComponent.h"
#include "GridMovementComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "SpriteComponent.h"
#include "Subject.h"
#include "ChaseGhostState.h"
#include "ScatterGhostState.h"
#include <iostream>
#include "PlayerComponent.h"
#include "RetryGhostState.h"

GhostComponent::GhostComponent(GameObject* owner, int gridX, int gridY,GhostType ghostType):BaseComponent(owner),
m_ghostType(ghostType)
{
	m_pSubject = std::make_unique<Subject>();
	m_pGridMovement = GetOwner()->AddComponent<GridMovementComponent>(gridX, gridY, 0);

	m_pGridMovement->SetSpeed(5.f);

	m_pGridMovement->SetDesiredDirection(glm::vec2(1,0));
	GameObject* parent = GetOwner()->GetParent();
	if (parent != nullptr)
	{
		SetLevelComponent(parent->GetComponent<LevelComponent>());
	}
	else
	{
		std::cout << "no parent found" << std::endl;
	}

	auto normalSpriteChild = GetOwner()->GetChildAt(0);
	if (normalSpriteChild)
	{
		m_pNormalSprite = normalSpriteChild->GetComponent<SpriteComponent>();

		if (m_pNormalSprite)
		{
			m_pNormalSprite->SetSpriteIndex(1, 0);
			m_pNormalSprite->PlaySpriteHor();
		}
		else {
			std::cout << "No m_pNormalSprite component found" << std::endl;
		}
	}
	auto scaredSpriteChild = GetOwner()->GetChildAt(1);
	if (scaredSpriteChild)
	{
		m_pScaredSprite = scaredSpriteChild->GetComponent<SpriteComponent>();

		if (m_pScaredSprite)
		{
			m_pScaredSprite->SetSpriteIndex(1, 0);
			m_pScaredSprite->PlaySpriteHor();
		}
		else {
			std::cout << "No m_pScaredSprite component found" << std::endl;
		}
	}

	auto DeadSpriteChild = GetOwner()->GetChildAt(2);
	if (DeadSpriteChild)
	{
		m_pDeadSprite = DeadSpriteChild->GetComponent<SpriteComponent>();

		if (m_pDeadSprite)
		{
			m_pDeadSprite->SetSpriteIndex(1, 0);
			m_pDeadSprite->PlaySpriteHor();
		}
		else {
			std::cout << "No m_pDeadSprite component found" << std::endl;
		}
	}
}

void GhostComponent::Notify(GameObject*, const std::string& event)
{
	if (event == "ActorDamaged")
	{
		RetryGhostState* pRetryState = new RetryGhostState();
		m_StateManager->PushState(pRetryState);
		m_ShouldMove = false;
	}

	if (event == "ActorRevived")
	{
		m_StateManager->PopState();
	}
}

void GhostComponent::Update(float deltaTime)
{
	if (!m_pLevelComponent || !m_pPacman || !m_pGridMovement) {
		return;
	}

	m_StateManager->Update(deltaTime);

	if (IsPlayerTouched())
	{
		auto playerComponent = m_pPacman->GetComponent<PlayerComponent>();
		if (playerComponent&& !playerComponent->IsDead())
		{
			playerComponent->LoseLife();
		}
	}

	if(m_DelayDecision)
		m_DelayTimer += deltaTime;

	if (m_DelayDecision&& m_DelayTimer >0.2f)
	{
		m_DelayTimer = 0.f;
		m_DelayDecision = false;
	}

	if(m_ShouldMove)
	{
		if (!m_DelayDecision)
		{
			glm::vec2 target = GetTargetPosition();

			glm::vec2 newDirection = ChooseBestDirection(target);

			glm::vec2 currentPos = m_pGridMovement->GetAccumulatedPosition();

			m_pGridMovement->SetDesiredDirection(newDirection);
		}

		m_pGridMovement->UpdatePosition(deltaTime);
	}
}

bool GhostComponent::IsPlayerTouched() const
{
	if (m_pPacman == nullptr) return false;

	float hitTreshold = m_pLevelComponent->TILE_WIDTH * 1.f;

	if (m_pPacman->GetWorldPosition().x < GetOwner()->GetWorldPosition().x + hitTreshold &&
		m_pPacman->GetWorldPosition().x > GetOwner()->GetWorldPosition().x - hitTreshold &&
		m_pPacman->GetWorldPosition().y < GetOwner()->GetWorldPosition().y + hitTreshold &&
		m_pPacman->GetWorldPosition().y > GetOwner()->GetWorldPosition().y - hitTreshold)
	{
		return true;
	}

	return false;
}

void GhostComponent::SetPacman(GameObject* pacman)
{
	m_pPacman = pacman;

	auto pChaseState = new ChaseGhostState();
	auto pScatterState = new ScatterGhostState();
	auto pScatterCondition = new ScatterCondition();
	auto pChaseCondition = new ChaseCondition();


	m_StateManager = std::make_unique<dae::StateManager<GhostComponent, GhostState>>(this, pScatterState);

	m_StateManager->AddTransition(pChaseState, pScatterState, pScatterCondition);
	m_StateManager->AddTransition(pScatterState, pChaseState, pChaseCondition);
}

void GhostComponent::SetLevelComponent(LevelComponent* levelComponent)
{
	m_pLevelComponent = levelComponent;
	if (m_pLevelComponent == nullptr)
	{
		std::cout << "no level found on parent" << std::endl;
		return;
	}
	m_pGridMovement->SetLevelComponent(m_pLevelComponent);

	const float TILE_WIDTH = m_pLevelComponent->TILE_WIDTH;

	switch (m_ghostType) {
	case GhostType::Red: m_scatterTarget = glm::vec2(25 * TILE_WIDTH, -6 * TILE_WIDTH); break;
	case GhostType::Pink:  m_scatterTarget = glm::vec2(2 * TILE_WIDTH, -6 * TILE_WIDTH); break;
	case GhostType::Cyan:   m_scatterTarget = glm::vec2(27 * TILE_WIDTH, 35 * TILE_WIDTH); break;
	case GhostType::Orange:  m_scatterTarget = glm::vec2(0, 35* TILE_WIDTH); break;
	}

	glm::vec2 borderSize = m_pLevelComponent->GetOwner()->GetWorldPosition();
	m_scatterTarget + borderSize;
}

glm::vec2 GhostComponent::ChooseBestDirection(const glm::vec2& target)
{
	if (m_pLevelComponent == nullptr)return glm::vec2(-1,0);

	glm::vec2 accumulatedPos = m_pGridMovement->GetAccumulatedPosition();

	int currentGridX =static_cast<int>(std::round(accumulatedPos.x));
	int currentGridY =static_cast<int>(std::round(accumulatedPos.y));

	glm::vec2 direction = m_lastDirection; // if no direction is found just use the last direction
 m_lastDirection;

	float bestDistance = FLT_MAX;

	for (auto dir : m_PossibleDirections)
	{
		if (dir == -m_lastDirection) continue;

		if ((currentGridX >= 12 && currentGridX <= 15) && currentGridY == 11 && (dir == m_PossibleDirections[0]|| dir == m_PossibleDirections[2]))continue;

		TileData nextTile = m_pLevelComponent->GetTile(currentGridX + static_cast<int>(dir.x), currentGridY + static_cast<int>(dir.y));

		if (nextTile.type == TileType::walls)continue;

		glm::vec3 vector = (GetOwner()->GetWorldPosition() + glm::vec3{ dir,0 }) - glm::vec3{ target,0 };
		
		float distance =glm::length(vector);
		if (distance < bestDistance)
		{
			bestDistance = distance;
			direction = dir;
		}
	}

	if (direction != m_lastDirection)
	{
		m_DelayDecision = true;
	}
	m_lastDirection = direction;
	return direction;
}
