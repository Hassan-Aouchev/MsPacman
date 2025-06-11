#include "Texture2DComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Texture2D.h"
#include <SDL.h>

void Texture2DComponent::Update(float)
{
}

struct Texture2DComponent::Impl
{
    SDL_Point rotationCenter{};
};

Texture2DComponent::Texture2DComponent(GameObject* pOwner)
    : BaseComponent(pOwner)
    , m_pImpl(std::make_unique<Impl>())
{
}

Texture2DComponent::~Texture2DComponent() = default;

void Texture2DComponent::SetRotationCenter(int x, int y)
{
    m_pImpl->rotationCenter.x = x;
    m_pImpl->rotationCenter.y = y;
    m_hasCustomCenter = true;
}

void Texture2DComponent::Render() const
{
    const auto& pos = BaseComponent::GetOwner()->GetWorldPosition();

    if (m_texture != nullptr)
    {
        const SDL_Point* centerPtr = m_hasCustomCenter ? &m_pImpl->rotationCenter : nullptr;

        SDL_RendererFlip sdlFlip = SDL_FLIP_NONE;
        switch (m_flip)
        {
        case FlipMode::Horizontal:
            sdlFlip = SDL_FLIP_HORIZONTAL;
            break;
        case FlipMode::Vertical:
            sdlFlip = SDL_FLIP_VERTICAL;
            break;
        case FlipMode::Both:
            sdlFlip = static_cast<SDL_RendererFlip>(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
            break;
        default:
            break;
        }

        Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y,
            static_cast<float>(m_texture->GetSize().x),
            static_cast<float>(m_texture->GetSize().y),
            m_Scale, m_rotation, centerPtr, sdlFlip);
    }
}



void Texture2DComponent::SetTexture(const std::string& filename)
{
    m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void Texture2DComponent::SetSprite(int rows, int cols)
{
    m_texture->SetSprite(rows, cols);
}

void Texture2DComponent::SetSpriteIndex(int index)
{
    m_texture->SetSpriteIndex(index);
}
