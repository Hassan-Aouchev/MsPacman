#include "HUDComponent.h"
#include "PlayerComponent.h"
#include "Text2DComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Texture2D.h"

HUDComponent::HUDComponent(GameObject* pOwner) : BaseComponent(pOwner)
{
	m_pLivesText1 = GetOwner()->GetChildAt(0)->GetComponent<Text2DComponent>();
	if (m_pLivesText1 != nullptr) m_pLivesText1->SetText("Lives: " + std::to_string(m_LivesPlayer1));

	m_pLivesText2 = GetOwner()->GetChildAt(2)->GetComponent<Text2DComponent>();
	if (m_pLivesText2 != nullptr) m_pLivesText2->SetText("Lives: " + std::to_string(m_LivesPlayer1));

	m_pScoreText1 = GetOwner()->GetChildAt(1)->GetComponent<Text2DComponent>();
	if (m_pScoreText1 != nullptr) m_pScoreText1->SetText("Score: " + std::to_string(m_ScorePlayer1));

	m_pScoreText2 = GetOwner()->GetChildAt(3)->GetComponent<Text2DComponent>();
	if (m_pScoreText2 != nullptr) m_pScoreText2->SetText("Score: " + std::to_string(m_ScorePlayer2));

	m_pLivesTexture = ResourceManager::GetInstance().LoadTexture("Lives.png");
}

void HUDComponent::Update(float )
{
}

void HUDComponent::Render() const
{
	int borderSize = Renderer::GetInstance().GetBorderSize() ;
	for (int i = 0; i < m_LivesPlayer1;++i) {

		Renderer::GetInstance().RenderTexture(*m_pLivesTexture,
			GetOwner()->GetWorldPosition().x+i* m_pLivesTexture->GetSize().x * 4.f + borderSize, // x position
			GetOwner()->GetWorldPosition().y + m_pLivesTexture->GetSize().y * 4.f + Renderer::GetInstance().GetScreenHeight()+6,// y position
			static_cast<float>(m_pLivesTexture->GetSize().x), static_cast<float>(m_pLivesTexture->GetSize().y), 4.0f);
	}
}


void HUDComponent::Notify(GameObject* object, const std::string& event)
{
	if (event == "ActorDamaged")
	{
		auto player = object->GetComponent<PlayerComponent>();
		if (player->GetPlayerIndex() == 0 && m_LivesPlayer1>0)
		{
			--m_LivesPlayer1;
			m_pLivesText1->SetText("Lives: " + std::to_string(m_LivesPlayer1));
		}
		else if (m_LivesPlayer2>0) {
			--m_LivesPlayer2;
			m_pLivesText1->SetText("Lives: " + std::to_string(m_LivesPlayer2));
		}
	}

	if (event == "DotEaten")
	{
		auto player = object->GetComponent<PlayerComponent>();
		if (player->GetPlayerIndex() == 0 )
		{
			m_ScorePlayer1 += 10;
			m_pScoreText1->SetText("Score: " + std::to_string(m_ScorePlayer1));
		}
		else{
			m_ScorePlayer2 += 10;
			m_pScoreText2->SetText("Score: " + std::to_string(m_ScorePlayer2));
		}
	}
}
