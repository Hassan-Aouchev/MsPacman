#include "MovementInputComponent.h"
#include "GameObject.h"
#include <iostream>

MovementInputComponent::MovementInputComponent(GameObject* pOwner):BaseComponent(pOwner),
m_Tolerance{0.001f},
m_MovementInput{ glm::vec2(0, 0) }
{
}

void MovementInputComponent::Update(float)
{
}

void MovementInputComponent::AddMovementInput(const glm::vec2& direction)
{
	// can only face one direction at a time so no need to add
	m_MovementInput = direction;
}

glm::vec2 MovementInputComponent::GetMovementInput()const 
{
	glm::vec2 movementDirection;
	if (std::abs(m_MovementInput.x) > 0.f || std::abs(m_MovementInput.y) > 0.f)
		movementDirection = glm::normalize(m_MovementInput);
	else
		movementDirection = glm::vec2(0, 0);
	return movementDirection;
}

