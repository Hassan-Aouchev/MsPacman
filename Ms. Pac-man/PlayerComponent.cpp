#include "PlayerComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "MovementInputComponent.h"
#include "SpriteComponent.h"
#include <ResourceManager.h>
#include "NormalPlayerState.h"
#include "DyingPlayerState.h"

PlayerComponent::PlayerComponent(GameObject* pOwner, int gridX, int gridY,
    int positionOffset, AudioService* audioService)
    : BaseComponent(pOwner)
    , m_AccumulatedPosition{ glm::vec2(gridX,gridY) }
    , m_GridX{ gridX }
    , m_GridY{ gridY }
    , m_PositionOffset{ positionOffset }
    , m_AudioService{ audioService }
    , WAKA_PATH{ "ms_eat_dot.wav" }
{
    m_pSubject = std::make_unique<Subject>();
	GameObject* parent = GetOwner()->GetParent();
	if (parent != nullptr)
	{
		SetLevelComponent(parent->GetComponent<LevelComponent>());
	}
	else
	{
		std::cout << "no parent found" << std::endl;
	}
	m_Lives = Lives(4);
	m_PlayerIndex = PlayerAmount;
	m_pMovementInput =  GetOwner()->GetComponent<MovementInputComponent>();
    auto spriteChild = GetOwner()->GetChildAt(0);
    if (spriteChild)
    {
        m_pSpriteComponent = spriteChild->GetComponent<SpriteComponent>();
    }
    if (m_pSpriteComponent)
    {
        m_pSpriteComponent->SetSpriteIndex(1, 0);
        m_pSpriteComponent->PlaySpriteHor();
    }
    else {
        std::cout << "No sprite component found" << std::endl;
    }

	++PlayerAmount;

    auto pNormalState = std::make_unique<NormalPlayerState>();
    auto pDyingState = std::make_unique<DyingPlayerState>();
    auto pDeathCondition = std::make_unique<DeathCondition>();

    NormalPlayerState* pStartState = pNormalState.get();

    m_StateManager = std::make_unique<dae::StateManager<PlayerComponent, PlayerState>>(this, pStartState);

    m_StateManager->AddTransition(std::move(pNormalState), std::move(pDyingState), std::move(pDeathCondition));

}

void PlayerComponent::Update(float deltaTime)
{
    m_StateManager->Update(deltaTime);

}

void PlayerComponent::MovePlayer(float x, float y)
{
	if (m_pLevelComponent == nullptr) return;
	GetOwner()->SetLocalPosition(glm::vec2(x * m_pLevelComponent->TILE_WIDTH + m_PositionOffset, y * m_pLevelComponent->TILE_WIDTH + m_PositionOffset));
	m_GridX = static_cast<int>((x));
	m_GridY = static_cast<int>((y));
}

void PlayerComponent::ProcessMovement(float deltaTime)
{
    if (m_pLevelComponent == nullptr) return;
    if (m_pSpriteComponent == nullptr) return;

    int roundedX = static_cast<int>(std::round(m_AccumulatedPosition.x));
    int roundedY = static_cast<int>(std::round(m_AccumulatedPosition.y));

    float diffX = std::abs(m_AccumulatedPosition.x - roundedX);
    float diffY = std::abs(m_AccumulatedPosition.y - roundedY);

    const glm::vec2 desiredDirection = m_pMovementInput->GetMovementInput();

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
            if (desiredDirection.x < 0.f)
            {
                m_pSpriteComponent->SetSpriteY(1);
            }
            else if (desiredDirection.x > 0.f)
            {
                m_pSpriteComponent->SetSpriteY(0);
            }
            m_pSpriteComponent->PlaySprite();
            m_PreviousDirection.y = 0.f;
            m_AccumulatedPosition.y = static_cast<float>(roundedY);
        }
        if (canMoveY)
        {
            m_PreviousDirection.y = desiredDirection.y;
            if (desiredDirection.y < 0.f)
            {
                m_pSpriteComponent->SetSpriteY(2);
            }
            else if (desiredDirection.y > 0.f)
            {
                m_pSpriteComponent->SetSpriteY(3);
            }
            m_pSpriteComponent->PlaySprite();
            m_PreviousDirection.x = 0.f;
            m_AccumulatedPosition.x = static_cast<float>(roundedX);
        }
        if (mustStopX)
        {
            m_PreviousDirection.x = 0.f;
            m_pSpriteComponent->StopSprite();
        }
        if (mustStopY)
        {
            m_PreviousDirection.y = 0.f;
            m_pSpriteComponent->StopSprite();
        }
    }
    m_AccumulatedPosition.x += m_PreviousDirection.x * deltaTime * m_Speed;
    m_AccumulatedPosition.y += m_PreviousDirection.y * deltaTime * m_Speed;
    MovePlayer(m_AccumulatedPosition.x, m_AccumulatedPosition.y);
}

void PlayerComponent::CheckDotCollection()
{
    int roundedX = static_cast<int>(std::round(m_AccumulatedPosition.x));
    int roundedY = static_cast<int>(std::round(m_AccumulatedPosition.y));

    if (m_pLevelComponent->GetTile(roundedX, roundedY).type == TileType::dots)
    {
        m_pLevelComponent->SetTile(roundedX, roundedY, TileType::empty);
        m_AudioService->PlaySound(WAKA_PATH);
    }
}

void PlayerComponent::SetLevelComponent(LevelComponent* levelComponent)
{
	m_pLevelComponent = levelComponent;
	if (m_pLevelComponent == nullptr)
	{
		std::cout << "no level found on parent" << std::endl;
	}
	MovePlayer(m_AccumulatedPosition.x, m_AccumulatedPosition.y);
}

void PlayerComponent::LoseLife()
{
    m_Lives.LoseLife();
    m_pSubject->NotifyObservers(GetOwner(), "ActorDamaged");
}
