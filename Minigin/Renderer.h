#pragma once
#include <SDL.h>
#include "Singleton.h"
#include <vector>

struct ImGuiContext;
class Texture2D;
/**
 * Simple RAII wrapper for the SDL renderer
 */
class Renderer final : public Singleton<Renderer>
{
	SDL_Renderer* m_renderer{};
	SDL_Window* m_window{};
	SDL_Color m_clearColor{};	


	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int BORDER_WIDTH;// for arcade type games with borders

public:
	void Init(SDL_Window* window,int screenWidth,int screenHeight,int borderSize);
	void Render() const;
	void Destroy();

	int GetScreenWidth()const { return SCREEN_WIDTH; }
	int GetScreenHeight()const { return SCREEN_HEIGHT; }
	int GetBorderSize()const { return BORDER_WIDTH; }

	void RenderTexture(const Texture2D& texture, float x, float y) const;
	void RenderTexture(const Texture2D& texture, float x, float y, float width, float height,float scale) const;

	void RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height, const float scale, const float rotation, const SDL_Point* center, const SDL_RendererFlip flip) const;

	SDL_Renderer* GetSDLRenderer() const;

	const SDL_Color& GetBackgroundColor() const { return m_clearColor; }
	void SetBackgroundColor(const SDL_Color& color) { m_clearColor = color; }
};

