#include "HUDComponent.h"
#include "PlayerComponent.h"
#include "Text2DComponent.h"

HUDComponent::HUDComponent(GameObject* pOwner) : BaseComponent(pOwner)
{
	m_pLivesText1 = GetOwner()->GetChildAt(0)->GetComponent<Text2DComponent>();
	if (m_pLivesText1 != nullptr) m_pLivesText1->SetText("Lives: " + std::to_string(m_LivesPlayer1));

	m_pLivesText2 = GetOwner()->GetChildAt(2)->GetComponent<Text2DComponent>();
	if (m_pLivesText2 != nullptr) m_pLivesText2->SetText("Lives: " + std::to_string(m_LivesPlayer1));

	m_pScoreText1 = GetOwner()->GetChildAt(3)->GetComponent<Text2DComponent>();
	if (m_pScoreText1 != nullptr) m_pScoreText1->SetText("Score: " + std::to_string(m_ScorePlayer1));

	m_pScoreText2 = GetOwner()->GetChildAt(1)->GetComponent<Text2DComponent>();
	if (m_pScoreText2 != nullptr) m_pScoreText2->SetText("Score: " + std::to_string(m_ScorePlayer2));
}

void HUDComponent::Update(float )
{
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

	if (event == "ScoreAdded")
	{
		auto player = object->GetComponent<PlayerComponent>();
		if (player->GetPlayerIndex() == 0 )
		{
			m_ScorePlayer1 += player->GetScore();
			m_pScoreText1->SetText("Score: " + std::to_string(m_ScorePlayer1));
		}
		else{
			m_ScorePlayer2 += player->GetScore();
			m_pScoreText2->SetText("Score: " + std::to_string(m_ScorePlayer2));
		}
	}
}
