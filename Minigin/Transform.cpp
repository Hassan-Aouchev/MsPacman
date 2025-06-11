#include "Transform.h"

void Transform::SetLocalPosition(const glm::vec3& position)
{
    m_LocalPosition = position;
}

void Transform::SetWorldPosition(const glm::vec3& position)
{
    m_WorldPosition = position;
}
