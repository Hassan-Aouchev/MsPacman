#pragma once
#include "BaseComponent.h"
#include <StateManager.h>
#include "Subject.h"
#include "AudioService.h"
#include "PlayerState.h"

class LevelComponent;
class MovementInputComponent;
class SpriteComponent;
class GridMovementComponent;

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
    GridMovementComponent* GetGridMovementComponent() const { return m_pGridMovement; }
    MovementInputComponent* GetMovementInput() const { return m_pMovementInput; }
    SpriteComponent* GetSpriteComponent() const { return m_pSpriteComponent; }
    AudioService* GetAudioService() const { return m_AudioService; }

    void ProcessMovement(float deltaTime);
    void CheckDotCollection();
    bool IsAlive() const { return m_Lives.IsAlive(); }

    void SetLevelComponent(LevelComponent* levelComponent);

    Subject* GetSubject() const { return m_pSubject.get(); }

    void LoseLife();
    int GetLives() const { return m_Lives.amount; }
    int GetPlayerIndex() const { return m_PlayerIndex; }

    const glm::vec2& GetFacingDirection()const { return m_FacingDirection; }

private:
    std::unique_ptr<dae::StateManager<PlayerComponent, PlayerState>> m_StateManager = nullptr;

    Lives m_Lives;
    int m_PlayerIndex{};

    std::unique_ptr<Subject> m_pSubject;
    LevelComponent* m_pLevelComponent = nullptr;
    MovementInputComponent* m_pMovementInput = nullptr;
    SpriteComponent* m_pSpriteComponent = nullptr;
    AudioService* m_AudioService = nullptr;

    glm::vec2 m_FacingDirection{};

    GridMovementComponent* m_pGridMovement = nullptr;

    const std::string WAKA_PATH;
};