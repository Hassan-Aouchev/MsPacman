#include "GridMovementComponent.h"
#include "LevelComponent.h"
#include "GameObject.h"

GridMovementComponent::GridMovementComponent(GameObject* owner, int gridX, int gridY, int positionOffset) : BaseComponent(owner),
m_GridX(gridX),
m_GridY(gridY),
m_PositionOffset(positionOffset),
m_AccumulatedPosition{ glm::vec2(gridX,gridY) }
{
}

void GridMovementComponent::SetLevelComponent(LevelComponent* pLevelComponent)
{
    m_pLevelComponent = pLevelComponent;
    MoveToPosition(m_AccumulatedPosition.x, m_AccumulatedPosition.y);
}

void GridMovementComponent::UpdatePosition(float deltaTime)
{
    if (m_pLevelComponent == nullptr) return;

    m_MovementAnim.Set(false, false, false);
    int roundedX = static_cast<int>(std::round(m_AccumulatedPosition.x));
    int roundedY = static_cast<int>(std::round(m_AccumulatedPosition.y));

    float diffX = std::abs(m_AccumulatedPosition.x - roundedX);
    float diffY = std::abs(m_AccumulatedPosition.y - roundedY);

    const glm::vec2 desiredDirection = m_DesiredDirection;

    const float forgivenessThreshold = 0.05f;

    if (diffX < forgivenessThreshold && diffY < forgivenessThreshold)
    {
        bool canMoveX = (m_pLevelComponent->GetTile(roundedX + static_cast<int>(desiredDirection.x), roundedY).type != TileType::walls);
        if (desiredDirection.x == 0.f) canMoveX = false;
        bool mustStopX = (m_pLevelComponent->GetTile(roundedX + static_cast<int>(m_PreviousDirection.x), roundedY).type == TileType::walls);

        bool canMoveY = (m_pLevelComponent->GetTile(roundedX, roundedY + static_cast<int>(desiredDirection.y)).type != TileType::walls);
        if (desiredDirection.y == 0.f) canMoveY = false;
        bool mustStopY = (m_pLevelComponent->GetTile(roundedX, roundedY + static_cast<int>(m_PreviousDirection.y)).type == TileType::walls);
        if (desiredDirection.y == 0.f) canMoveY = false;

        if (canMoveX)
        {
            m_PreviousDirection.x = desiredDirection.x;
            m_PreviousDirection.y = 0.f;
            m_AccumulatedPosition.y = static_cast<float>(roundedY);
        }
        if (canMoveY)
        {
            m_PreviousDirection.y = desiredDirection.y;
            m_PreviousDirection.x = 0.f;
            m_AccumulatedPosition.x = static_cast<float>(roundedX);
        }
        if (mustStopX)
        {
            m_PreviousDirection.x = 0.f;
        }
        if (mustStopY)
        {
            m_PreviousDirection.y = 0.f;
        }

        m_MovementAnim.Set(canMoveX,canMoveY,(mustStopY||mustStopX));
    }
    if (std::abs(m_PreviousDirection.x) > 0.f)
    {
        bool ye = true;
        ye = false;
    }
    m_AccumulatedPosition.x += m_PreviousDirection.x * deltaTime * m_Speed;
    m_AccumulatedPosition.y += m_PreviousDirection.y * deltaTime * m_Speed;
    MoveToPosition(m_AccumulatedPosition.x, m_AccumulatedPosition.y);
}

void GridMovementComponent::MoveToPosition(float x, float y)
{
	if (m_pLevelComponent == nullptr) return;
	GetOwner()->SetLocalPosition(glm::vec2(x * m_pLevelComponent->TILE_WIDTH + m_PositionOffset, y * m_pLevelComponent->TILE_WIDTH + m_PositionOffset));
	m_GridX = static_cast<int>((x));
	m_GridY = static_cast<int>((y));
}

bool GridMovementComponent::CanMoveInDirection(const glm::vec2& , int , int ) const
{
    return false;
}
