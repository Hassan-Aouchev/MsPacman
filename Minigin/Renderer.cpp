#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui.h"
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void Renderer::Init(SDL_Window* window, int screenWidth, int screenHeight, int borderSize)
{
	m_window = window;
	m_renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	SCREEN_WIDTH = screenWidth;
	SCREEN_HEIGHT = screenHeight;
	BORDER_WIDTH = borderSize;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(m_window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_renderer);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	
	SceneManager::GetInstance().Render();

	ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	SDL_RenderPresent(m_renderer);
}

void Renderer::Destroy()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_renderer != nullptr)
	{
		SDL_DestroyRenderer(m_renderer);
		m_renderer = nullptr;
	}
}


void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height,const float scale,int spriteIndex) const
{

	int spriteWidth =  static_cast<int>(width) / texture.GetRows();
	int spriteHeight = static_cast<int>(height) / (texture.GetCols());
	int row = spriteIndex % texture.GetRows();
	int col = spriteIndex / texture.GetRows();

	SDL_Rect dst1{};
	dst1.x = static_cast<int>(row*spriteWidth);
	dst1.y = static_cast<int>(col * spriteHeight);
	dst1.w = static_cast<int>(spriteWidth);
	dst1.h = static_cast<int>(spriteHeight);


	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(spriteWidth *scale);
	dst.h = static_cast<int>(spriteHeight *scale);

	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(),&dst1, &dst);
}

void Renderer::RenderTexture(const Texture2D& texture, const float x, const float y,
	const float width, const float height, const float scale,
	const float rotation = 0.0f,
	const SDL_Point* center = nullptr,
	const SDL_RendererFlip flip = SDL_FLIP_NONE,int spriteIndex) const
{
	int spriteWidth = static_cast<int>(width) / texture.GetRows();
	int spriteHeight = static_cast<int>(height) / texture.GetCols();
	int row = spriteIndex % texture.GetRows();
	int col = spriteIndex / texture.GetRows();

	SDL_Rect src{};
	src.x = static_cast<int>(row * spriteWidth);
	src.y = static_cast<int>(col * spriteHeight);
	src.w = static_cast<int>(spriteWidth);
	src.h = static_cast<int>(spriteHeight);

	SDL_Rect dst{};
	dst.x = static_cast<int>(x);
	dst.y = static_cast<int>(y);
	dst.w = static_cast<int>(spriteWidth * scale);
	dst.h = static_cast<int>(spriteHeight * scale);

	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &src, &dst,
		rotation, center, flip);
}

SDL_Renderer* Renderer::GetSDLRenderer() const { return m_renderer; }
