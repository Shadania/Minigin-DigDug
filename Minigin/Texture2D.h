#pragma once
struct SDL_Texture;

namespace dae
{
	class Texture2D
	{
	public:
		explicit Texture2D(SDL_Texture* texture);
		~Texture2D();

		Texture2D(const Texture2D &) = delete;
		Texture2D(Texture2D &&) = delete;
		Texture2D & operator= (const Texture2D &) = delete;
		Texture2D & operator= (const Texture2D &&) = delete;

		SDL_Texture* GetSDLTexture() const;
		int GetWidth() const;
		int GetHeight() const;


	private:
		SDL_Texture* m_Tex;
		uint32_t m_Format;
		int m_Access;
		int m_Width;
		int m_Height;
	};
}
