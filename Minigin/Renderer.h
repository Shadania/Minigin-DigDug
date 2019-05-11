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
		void Init(SDL_Window* window);
		void Render();
		void Destroy();
		void SetScale(float scale);

		void RenderTexture(const Texture2D& texture, float x, float y) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTexture(const Texture2D& tex, const Float4& destRect, const Float4& srcRect) const;

		SDL_Renderer* GetSDLRenderer() const { return mRenderer; }

	private:
		SDL_Renderer* mRenderer = nullptr;
		float m_Scale;
	};
}

// These object pools don't seem to like smart pointers. Exceptions are thrown on program end.