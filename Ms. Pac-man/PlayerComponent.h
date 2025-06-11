#pragma once
#include "BaseComponent.h"
#include <StateManager.h>
#include "Subject.h"
#include "AudioService.h"
#include "PlayerState.h"

class LevelComponent;
class MovementInputComponent;
class SpriteComponent;

static int PlayerAmount{};

struct Lives final
{
    Lives(int initialLives = 3) : amount(initialLives) {}
    void LoseLife() { if (amount > 0) --amount; }
    bool IsAlive() const { return amount > 0; }
    int amount;
};

class PlayerComponent : public BaseComponent
{
public:
    PlayerComponent(GameObject* pOwner, int gridX, int gridY, int positionOffset, AudioService* audioService);

    void Update(float deltaTime) override;
    void Render() const override {}

    LevelComponent* GetLevelComponent() const { return m_pLevelComponent; }
    MovementInputComponent* GetMovementInput() const { return m_pMovementInput; }
    SpriteComponent* GetSpriteComponent() const { return m_pSpriteComponent; }
    AudioService* GetAudioService() const { return m_AudioService; }

    const glm::vec2& GetAccumulatedPosition() const { return m_AccumulatedPosition; }
    void SetAccumulatedPosition(const glm::vec2& pos) { m_AccumulatedPosition = pos; }

    const glm::vec2& GetPreviousDirection() const { return m_PreviousDirection; }
    void SetPreviousDirection(const glm::vec2& dir) { m_PreviousDirection = dir; }

    int GetGridX() const { return m_GridX; }
    int GetGridY() const { return m_GridY; }

    void MovePlayer(float x, float y);
    void ProcessMovement(float deltaTime);
    void CheckDotCollection();
    bool IsAlive() const { return m_Lives.IsAlive(); }

    void SetLevelComponent(LevelComponent* levelComponent);

    Subject* GetSubject() const { return m_pSubject.get(); }

    void LoseLife();
    void AddScore() { }
    int GetScore() const { return 10; }
    int GetLives() const { return m_Lives.amount; }
    int GetPlayerIndex() const { return m_PlayerIndex; }
    void SetSpeed(float speed) { m_Speed = speed; }
    float GetSpeed() const { return m_Speed; }

private:
    std::unique_ptr<dae::StateManager<PlayerComponent, PlayerState>> m_StateManager = nullptr;

    glm::vec2 m_AccumulatedPosition{};
    glm::vec2 m_PreviousDirection{};
    int m_GridX{};
    int m_GridY{};
    const int m_PositionOffset{};
    Lives m_Lives;
    int m_PlayerIndex{};
    float m_Speed = 5.0f;

    std::unique_ptr<Subject> m_pSubject;
    LevelComponent* m_pLevelComponent = nullptr;
    MovementInputComponent* m_pMovementInput = nullptr;
    SpriteComponent* m_pSpriteComponent = nullptr;
    AudioService* m_AudioService = nullptr;

    const std::string WAKA_PATH;
};