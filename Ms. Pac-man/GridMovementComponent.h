#pragma once
#include <BaseComponent.h>
#include <glm.hpp>

struct MovementAnim {
    bool mustStop = false;
    bool moveY = false;
    bool moveX = false;

    void Set(bool x, bool y, bool stop) {
        moveX = x;
        moveY = y;
        mustStop = stop;
    }
};

class LevelComponent;
class GridMovementComponent :
    public BaseComponent
{
public:
    GridMovementComponent(GameObject* owner, int gridX, int gridY, int positionOffset);

    void Update(float) override {};
    void Render() const override {};

    void SetDesiredDirection(const glm::vec2& direction) { m_DesiredDirection = direction; }
    void SetSpeed(float speed) { m_Speed = speed; }
    float GetSpeed()const { return m_Speed; }

    const glm::vec2& GetAccumulatedPosition() const { return m_AccumulatedPosition; }
    void SetAccumulatedPosition(const glm::vec2& pos) { m_AccumulatedPosition = pos; }
    const glm::vec2& GetPreviousDirection() const { return m_PreviousDirection; }
    const glm::vec2& GetDesiredDirection()const { return m_DesiredDirection; }
    int GetGridX() const { return m_GridX; }
    int GetGridY() const { return m_GridY; }

    void SetLevelComponent(LevelComponent* pLevelComponent) { m_pLevelComponent = pLevelComponent; }
    LevelComponent* GetLevelComponent() const { return m_pLevelComponent; }

    const MovementAnim& GetMovementAnim()const { return m_MovementAnim; }
    void ShouldMove(bool shouldMove) { m_ShouldMove = shouldMove; }
    void UpdatePosition(float deltaTime);

private:
    void MoveToPosition(float x, float y);
    bool CanMoveInDirection(const glm::vec2& direction, int roundedX, int roundedY)const;

    glm::vec2 m_AccumulatedPosition;
    glm::vec2 m_PreviousDirection{};
    glm::vec2 m_DesiredDirection{};
    int m_GridX{};
    int m_GridY{};
    const int m_PositionOffset{};
    float m_Speed{};

    bool m_ShouldMove = false;

    MovementAnim m_MovementAnim{};

    LevelComponent* m_pLevelComponent = nullptr;

};

