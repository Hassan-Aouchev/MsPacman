#include "SpriteComponent.h"
#include "Texture2DComponent.h"
#include "GameObject.h"

SpriteComponent::SpriteComponent(GameObject* pOwner, int rows, int cols, float speed):BaseComponent(pOwner),
m_Rows{rows},
m_Cols{cols},
m_Speed{speed}
{
	m_pSpriteTexture = GetOwner()->GetComponent<Texture2DComponent>();
	if (m_pSpriteTexture)
	{
		m_pSpriteTexture->SetSprite(rows, cols);
	}
}

void SpriteComponent::Update(float deltaTime)
{
	if (!m_PlaySprite) return;
	if (m_pSpriteTexture == nullptr) return;
	int accumuIndex = static_cast<int>(m_AccumelatedTime * m_Speed);
	m_AccumelatedTime += deltaTime;
	if (m_PlaySpriteHor&& !m_PlaySpriteVert)
	{
		m_X = accumuIndex % m_Rows;
	}
	else if (m_PlaySpriteVert&&!m_PlaySpriteHor)
	{
		m_Y = accumuIndex % m_Cols;
	}
	else
	{
		m_X = accumuIndex % m_Rows;
		m_Y = accumuIndex / m_Rows;
	}
	SetSpriteIndex(m_X,m_Y);
}

void SpriteComponent::SetSpriteIndex(int x, int y)
{
	if (m_pSpriteTexture == nullptr) return;
	m_pSpriteTexture->SetSpriteIndex(x+ y*m_Rows);
	m_X = x;
	m_Y = y;
	//if (m_PlaySpriteHor && !m_PlaySpriteVert)
	//{
	//	m_AccumelatedTime = m_X / m_Speed;
	//}
	//else if (m_PlaySpriteVert && !m_PlaySpriteHor)
	//{
	//	m_AccumelatedTime = m_Y / m_Speed;
	//}
	//else
	//{
	//	m_AccumelatedTime = (x + y * m_Rows) / m_Speed;
	//}
}

void SpriteComponent::SetSpriteY(int y)
{
	SetSpriteIndex(m_X, y);
}

void SpriteComponent::SetSpriteX(int x)
{
	SetSpriteIndex(x, m_Y);
}

