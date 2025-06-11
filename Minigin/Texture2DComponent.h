#pragma once
#include <string>
#include <memory>
#include "BaseComponent.h"

class Texture2D;

class Texture2DComponent : public BaseComponent
{
public:
    Texture2DComponent(GameObject* pOwner);
    ~Texture2DComponent() override;

    void Update(float deltaTime) override;
    void Render() const override;

    void SetTexture(const std::string& filename);
    void SetScale(float scale) { m_Scale = scale; }
    void SetSprite(int rows, int cols);
    void SetSpriteIndex(int index);

    void SetRotation(float degrees) { m_rotation = degrees; }
    float GetRotation() const { return m_rotation; }
    void Rotate(float degrees) { m_rotation += degrees; }

    void SetRotationCenter(int x, int y);
    void ClearRotationCenter() { m_hasCustomCenter = false; }

    enum class FlipMode
    {
        None = 0,
        Horizontal = 1,
        Vertical = 2,
        Both = 3
    };
    void SetFlip(FlipMode flip) { m_flip = flip; }
    FlipMode GetFlip() const { return m_flip; }

private:
    struct Impl;
    std::unique_ptr<Impl> m_pImpl;

    std::shared_ptr<Texture2D> m_texture{};
    float m_Scale{ 1.0f };
    float m_rotation = 0.0f;
    bool m_hasCustomCenter = false;
    FlipMode m_flip = FlipMode::None;
};