#pragma once
#include <BaseComponent.h>
#include <StateManager.h>
#include "GhostState.h"
#include <array>
#include <glm.hpp>
class Subject;
class SpriteComponent;
class AudioService;
class GridMovementComponent;
class LevelComponent;

static std::array<glm::vec2, 4> m_PossibleDirections{
    glm::vec2(0, -1) ,
    glm::vec2(-1, 0),
    glm::vec2(0, 1),
    glm::vec2(1, 0) };

enum class GhostType {
    Red,
    Pink,
    Orange,
    Cyan
};

class GhostComponent :
    public BaseComponent
{
public:
    GhostComponent(GameObject* owner, int gridX, int gridY, GhostType ghostType);

    void Update(float deltaTime) override;

    void Render() const override {};

    void SetPacman(GameObject* pacman);
    GameObject* GetPacman()const { return m_pPacman; }
    void SetBlinky(GameObject* blinky) { m_pBlinkyGhost = blinky; }
    GameObject* GetBlinky()const { return m_pBlinkyGhost; }
    

    GridMovementComponent* GetGridMovementComponent()const { return m_pGridMovement; }

    SpriteComponent* const GetNormalSprite()const { return m_pNormalSprite; }

    GhostType GetGhostType() const { return m_ghostType; }

    const glm::vec2& GetTargetPosition()const { return m_TargetPosition; }

    void SetTargetPosition(const glm::vec2& pos) { m_TargetPosition = pos; }

    const glm::vec2& GetScatterPosition()const { return m_scatterTarget; }

private:
    void SetLevelComponent(LevelComponent* levelComponent);

    std::unique_ptr<dae::StateManager<GhostComponent, GhostState>> m_StateManager = nullptr;

    GridMovementComponent* m_pGridMovement = nullptr;
    std::unique_ptr<Subject> m_pSubject;
    LevelComponent* m_pLevelComponent = nullptr;
    //MovementInputComponent* m_pMovementInput = nullptr;
    SpriteComponent* m_pNormalSprite = nullptr;
    SpriteComponent* m_pScaredSprite = nullptr;
    SpriteComponent* m_pDeadSprite = nullptr;

    GhostType m_ghostType{GhostType::Red};
    glm::vec2 m_scatterTarget;
    glm::vec2 m_lastDirection;
    glm::vec2 m_TargetPosition;
    float m_DelayTimer{};

    GameObject* m_pPacman;
    GameObject* m_pBlinkyGhost;

    int m_LastGridX{};
    int m_LastGridY{};
    glm::vec2 ChooseBestDirection(const glm::vec2& target);

    bool m_DelayDecision{ false };
};

