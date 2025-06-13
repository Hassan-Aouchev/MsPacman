#include "PlayerComponent.h"
#include "GameObject.h"
#include "LevelComponent.h"
#include "MovementInputComponent.h"
#include "SpriteComponent.h"
#include <ResourceManager.h>
#include "NormalPlayerState.h"
#include "DyingPlayerState.h"
#include "GridMovementComponent.h"

PlayerComponent::PlayerComponent(GameObject* pOwner, int gridX, int gridY, int positionOffset, AudioService* audioService)
    : BaseComponent(pOwner)
    , m_AudioService{ audioService }
    , WAKA_PATH{ "ms_eat_dot.wav" }
{
    m_pGridMovement = GetOwner()->AddComponent<GridMovementComponent>(gridX, gridY, positionOffset);

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

        if (m_pSpriteComponent)
        {
            m_pSpriteComponent->SetSpriteIndex(1, 0);
            m_pSpriteComponent->PlaySpriteHor();
        }
        else {
            std::cout << "No sprite component found" << std::endl;
        }
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

void PlayerComponent::ProcessMovement(float )
{
    if (m_pLevelComponent == nullptr) return;
    if (m_pSpriteComponent == nullptr) return;
    if (m_pGridMovement == nullptr) return;

    const glm::vec2& desiredDirection = m_pGridMovement->GetDesiredDirection();

    const MovementAnim& moveAnim = m_pGridMovement->GetMovementAnim();

    if (moveAnim.moveX)
    {
        if (desiredDirection.x < 0.f)
        {
            m_pSpriteComponent->SetSpriteY(1);
            m_FacingDirection = glm::vec2(-1, 0);
        }
        else if (desiredDirection.x > 0.f)
        {
            m_pSpriteComponent->SetSpriteY(0);
            m_FacingDirection = glm::vec2(1, 0);
        }
        m_pSpriteComponent->PlaySprite();
    }
    if (moveAnim.moveY)
    {
        if (desiredDirection.y < 0.f)
        {
            m_pSpriteComponent->SetSpriteY(2);
            m_FacingDirection = glm::vec2(0, -1);
        }
        else if (desiredDirection.y > 0.f)
        {
            m_pSpriteComponent->SetSpriteY(3);
            m_FacingDirection = glm::vec2(0, 1);
        }
        m_pSpriteComponent->PlaySprite();
    }
    if (moveAnim.mustStop)
    {
        m_pSpriteComponent->StopSprite();
    }
}

void PlayerComponent::CheckDotCollection()
{
    auto accumulatedPos = m_pGridMovement->GetAccumulatedPosition();

    int roundedX = static_cast<int>(std::round(accumulatedPos.x));
    int roundedY = static_cast<int>(std::round(accumulatedPos.y));

    if (m_pLevelComponent->GetTile(roundedX, roundedY).type == TileType::dots)
    {
        m_pLevelComponent->SetTile(roundedX, roundedY, TileType::empty);
        m_AudioService->PlaySound(WAKA_PATH);
        GetSubject()->NotifyObservers(GetOwner(), "DotEaten");
    }
}

void PlayerComponent::SetLevelComponent(LevelComponent* levelComponent)
{
	m_pLevelComponent = levelComponent;
	if (m_pLevelComponent == nullptr)
	{
		std::cout << "no level found on parent" << std::endl;
	}
    m_pGridMovement->SetLevelComponent(levelComponent);
}

void PlayerComponent::LoseLife()
{
    m_Lives.LoseLife();
    m_pSubject->NotifyObservers(GetOwner(), "ActorDamaged");
}
