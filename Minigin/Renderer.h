#pragma once
#include "TextureComponents.h"
#include "ObjectPool.h"

struct SDL_Window;
struct SDL_Renderer;

namespace dae
{
	class Texture2D;
	class Renderer
	{

	public:
		Renderer();
		void Init(SDL_Window* window);
		void Render();
		void Destroy();
		void SetScale(float scale);

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& tex, const Float4& destRect, const Float4& srcRect) const;

		SDL_Renderer* GetSDLRenderer() const { return mRenderer; }

		// std::shared_ptr<dae::TextureComponent> CreateTextureComponent(std::shared_ptr<GameObject> pObj, const std::string& filePath);
		// std::shared_ptr<dae::TextComponent> CreateTextComponent(std::shared_ptr<GameObject> pObj, const std::string& text, const std::shared_ptr<dae::Font>& font);

	private:
		SDL_Renderer* mRenderer = nullptr;
		float m_Scale;
		// ObjectPool<TextureComponent> m_TextureComponentPool;
		// ObjectPool<TextComponent> m_TextComponentPool;
	};
}

// These object pools don't seem to like smart pointers. Exceptions are thrown on program end.