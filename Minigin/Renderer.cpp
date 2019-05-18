#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include "Font.h"
#include "ServiceLocator.h"


dae::Renderer::Renderer()
	:m_Scale{1.0f}
{

}

dae::Renderer::~Renderer()
{
	Destroy();
}

void dae::Renderer::Init(SDL_Window * window)
{
	m_pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (m_pRenderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void dae::Renderer::Render()
{
	SDL_RenderClear(m_pRenderer);

	ServiceLocator::GetSceneManager()->Render();
	
	SDL_RenderPresent(m_pRenderer);
}

void dae::Renderer::Destroy()
{
	if (m_pRenderer)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
}
void dae::Renderer::SetScale(float scale)
{
	m_Scale = scale;
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{int(x * m_Scale), int(y * m_Scale), int(texture.GetWidth() * m_Scale), int(texture.GetHeight() * m_Scale)};
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}
void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{int(x * m_Scale), int(y * m_Scale), int(width * m_Scale), int (height * m_Scale)};
	// dst.x = static_cast<int>(x);
	// dst.y = static_cast<int>(y);
	// dst.w = static_cast<int>(width);
	// dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}
void dae::Renderer::RenderTexture(const Texture2D& tex, const Float4& destRect, const Float4& srcRect) const
{
	SDL_Rect destRectSDL{ int(destRect.x * m_Scale), int(destRect.y * m_Scale), int(destRect.z * m_Scale), int(destRect.w * m_Scale) };
	SDL_Rect srcRectSDL{ int(srcRect.x), int(srcRect.y), int(srcRect.z), int(srcRect.w) };
	SDL_RenderCopy(GetSDLRenderer(), tex.GetSDLTexture(), &srcRectSDL, &destRectSDL);
}
void dae::Renderer::RenderTexture(const Texture2D& tex, const Float4& destRect)
{
	Float4 srcRect{};
	srcRect.x = 0.0f;
	srcRect.y = 0.0f;
	srcRect.z = (float)tex.GetWidth();
	srcRect.w = (float)tex.GetHeight();
	RenderTexture(tex, destRect, srcRect);
}


void dae::Renderer::RenderColorRect(const Float4&, const Float4&) const
{
	// SDL_Rect destRectSDL{ int(destRect.x * m_Scale), int(destRect.y * m_Scale), int(destRect.z * m_Scale), int(destRect.w * m_Scale) };
	// SDL_SetRenderDrawColor(GetSDLRenderer(), Uint8(color.x), Uint8(color.y), Uint8(color.z), Uint8(color.w));
	// if (SDL_RenderFillRect(GetSDLRenderer(), &destRectSDL) != 0)
	// {
	// 	std::cout << SDL_GetError() << std::endl;
	// }
	// 
	// SDL_RenderDrawRect(GetSDLRenderer(), NULL);

	// Doesn't seem to work
	// maybe figure it out later
}