#pragma once
#include <glm.hpp>

class Transform final
{
public:
	void SetLocalPosition(const glm::vec3& position);
	//void SetLocalRotation(float x, float y, float z);
	//void SetLocalScale(float x, float y, float z);
	const glm::vec3& GetLocalPosition() const { return m_LocalPosition; }
	//const glm::vec3& GetLocalRotation() const { return m_LocalRotation; }
	//const glm::vec3& GetLocalScale() const { return m_LocalScale; }

	void SetWorldPosition(const glm::vec3& position);
	//void SetWorldRotation(float x, float y, float z);
	//void SetWorldScale(float x, float y, float z);
	const glm::vec3& GetWorldPosition() const { return m_WorldPosition; }
	//const glm::vec3& GetWorldRotation() const { return m_WorldRotation; }
	//const glm::vec3& GetWorldScale() const { return m_WorldScale; }
private:
	glm::vec3 m_LocalPosition;
	//glm::vec3 m_LocalScale;
	//glm::vec3 m_LocalRotation;
	glm::vec3 m_WorldPosition;
	//glm::vec3 m_WorldScale;
	//glm::vec3 m_WorldRotation;
};
