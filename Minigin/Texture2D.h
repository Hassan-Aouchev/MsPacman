#pragma once
#include <vec2.hpp>
#include <string>

struct SDL_Texture;

class Texture2D final
{
public:
    SDL_Texture* GetSDLTexture() const;
    explicit Texture2D(SDL_Texture* texture);
    explicit Texture2D(const std::string& fullPath);
    ~Texture2D();

    glm::ivec2 GetSize() const;

    void SetSpriteIndex(int index);

    Texture2D(const Texture2D&) = delete;
    Texture2D(Texture2D&&) = delete;
    Texture2D& operator= (const Texture2D&) = delete;
    Texture2D& operator= (const Texture2D&&) = delete;
    void SetSprite(int rows, int cols);

    int GetRows() const { return m_rows; }
    int GetCols() const { return m_cols; }

private:
    int m_rows{ 1 };
    int m_cols{ 1 };

    SDL_Texture* m_texture;
};