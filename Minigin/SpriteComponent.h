#pragma once
#include "BaseComponent.h"
class Texture2DComponent;
class SpriteComponent: public BaseComponent
{
public:

    SpriteComponent(GameObject* pOwner,int rows, int cols,float speed);
    ~SpriteComponent() override = default;
    void Update(float deltaTime) override;
    void Render() const override {}
    void SetSpriteIndex(int x, int y);
    void SetSpriteY(int y);
    void SetSpriteX(int x);
    void PlaySprite() { m_PlaySprite = true; }
    void StopSprite() { m_PlaySprite = false; }
    void PlaySpriteVert() { m_PlaySpriteVert = true; }
    void PlaySpriteHor() { m_PlaySpriteHor = true; }
    void StopSpriteVert() { m_PlaySpriteVert = false; }
    void StopSpriteHor() { m_PlaySpriteHor = false; }
    Texture2DComponent* GetTextureComp() const { return m_pSpriteTexture; }
private:
    int m_X{};
    int m_Y{};
    int m_Rows{1};
    int m_Cols{1};
    float m_Speed;
    float m_AccumelatedTime{};
    Texture2DComponent* m_pSpriteTexture{};
    bool m_PlaySpriteHor{};
    bool m_PlaySpriteVert{};
    bool m_PlaySprite{};
};

