#pragma once
#include "BaseComponent.h"
#include <string>
class Font;
class Texture2D;
class Text2DComponent : public BaseComponent
{
public:

    Text2DComponent(GameObject* pOwner,const std::string& text, std::shared_ptr<Font> font);
    ~Text2DComponent() override = default;
    Text2DComponent(const Text2DComponent& other) = delete;
    Text2DComponent(Text2DComponent&& other) noexcept = delete;
    Text2DComponent& operator=(const Text2DComponent& other) = delete;
    Text2DComponent& operator=(Text2DComponent&& other) noexcept = delete;
    
    void Update(float deltaTime) override;
    void Render() const override;

    void SetText(const std::string& text);
private:
    bool m_needsUpdate;
    std::string m_text;
    std::shared_ptr<Font> m_font;
    std::shared_ptr<Texture2D> m_textTexture;
};
