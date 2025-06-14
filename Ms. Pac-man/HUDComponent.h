#pragma once
#include "BaseComponent.h"
#include "Observer.h"

class Text2DComponent;
class Texture2D;

class HUDComponent:public BaseComponent, public Observer
{
public:
	HUDComponent(GameObject* pOwner);

	void Update(float elapsedSec) override;
	void Render()const override;

	virtual void Notify(GameObject* object,const std::string& event) override;
private:
	int m_LivesPlayer1 = { 4 };
	int m_LivesPlayer2 = { 4 };
	int m_ScorePlayer1 = { 0 };
	int m_ScorePlayer2 = { 0 };
	Text2DComponent* m_pLivesText1;
	Text2DComponent* m_pLivesText2;

	Text2DComponent* m_pScoreText1;
	Text2DComponent* m_pScoreText2;

	std::shared_ptr<Texture2D> m_pLivesTexture;
};

