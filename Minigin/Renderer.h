#pragma once
#include "TextureComponents.h"

struct SDL_Window;
struct SDL_Renderer;

namespace dae
{
	class Texture2D;
	class Renderer
	{

	public:
		Renderer();
		~Renderer();

		void Init(SDL_Window* window);
		void Render();
		void SetScale(float scale);

		void RenderTextureFullScreen(const Texture2D& texture) const;
		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& tex, const Float4& destRect, const Float4& srcRect) const;
		void RenderTexture(const Texture2D& tex, const Float4& destRect);

		SDL_Renderer* GetSDLRenderer() const { return m_pRenderer; }

	private:
		SDL_Renderer* m_pRenderer = nullptr;
		void Destroy();
		float m_Scale;
	};
}