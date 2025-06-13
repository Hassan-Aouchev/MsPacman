#include "GameObject.h"
#include "PacmanCommands.h"
#include "MovementInputComponent.h"
#include "PlayerComponent.h"
Move::Move(GameObject* gameObject, glm::vec2 direction) :GameObjectCommand(gameObject)
{
	m_direction = direction;
}
void Move::Execute()
{
	GetGameObject()->GetComponent<MovementInputComponent>()->AddMovementInput(m_direction);
}

Damage::Damage(GameObject* gameObject) :GameObjectCommand(gameObject)
{
}

void Damage::Execute()
{
	GetGameObject()->GetComponent<PlayerComponent>()->LoseLife();
}

Score::Score(GameObject* gameObject) : GameObjectCommand(gameObject)
{
}

void Score::Execute()
{
}
