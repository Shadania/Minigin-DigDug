#include "MiniginPCH.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include "Font.h"
#include "ServiceLocator.h"


dae::Renderer::Renderer()
	:m_Scale{1.0f}
	// : m_TextureComponentPool{20}
	// , m_TextComponentPool{20}
{

}


void dae::Renderer::Init(SDL_Window * window)
{
	mRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (mRenderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}
}

void dae::Renderer::Render()
{
	SDL_RenderClear(mRenderer);

	ServiceLocator::GetSceneManager()->Render();
	
	SDL_RenderPresent(mRenderer);
}

void dae::Renderer::Destroy()
{
	if (mRenderer != nullptr)
	{
		SDL_DestroyRenderer(mRenderer);
		mRenderer = nullptr;
	}
}
void dae::Renderer::SetScale(float scale)
{
	m_Scale = scale;
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dst{int(x), int(y), int(texture.GetWidth() * m_Scale), int(texture.GetHeight() * m_Scale)};
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float width, const float height) const
{
	SDL_Rect dst{int(x), int(y), int(width * m_Scale), int (height * m_Scale)};
	// dst.x = static_cast<int>(x);
	// dst.y = static_cast<int>(y);
	// dst.w = static_cast<int>(width);
	// dst.h = static_cast<int>(height);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dst);
}

void dae::Renderer::RenderTexture(const Texture2D& tex, const Float4& destRect, const Float4& srcRect) const
{
	SDL_Rect destRectSDL{ int(destRect.x), int(destRect.y), int(destRect.z * m_Scale), int(destRect.w * m_Scale) };
	SDL_Rect srcRectSDL{ int(srcRect.x), int(srcRect.y), int(srcRect.z), int(srcRect.w) };
	SDL_RenderCopy(GetSDLRenderer(), tex.GetSDLTexture(), &srcRectSDL, &destRectSDL);
}


/*
std::shared_ptr<dae::TextureComponent> dae::Renderer::CreateTextureComponent(std::shared_ptr<GameObject> pObj, const std::string& filePath)
{
	std::shared_ptr<TextureComponent> newTexComp{ m_TextureComponentPool.RequestMemory() };
	newTexComp->SetTexture(filePath);
	newTexComp->SetGameObj(pObj);
	return newTexComp;
}
std::shared_ptr<dae::TextComponent> dae::Renderer::CreateTextComponent(std::shared_ptr<GameObject> pObj, const std::string& text, const std::shared_ptr<dae::Font>& font)
{
	std::shared_ptr<TextComponent> newTextComp{ m_TextComponentPool.RequestMemory() };
	newTextComp->SetFont(font);
	newTextComp->SetText(text);
	newTextComp->SetGameObj(pObj);

	return newTextComp;
}

*/