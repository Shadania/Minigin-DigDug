#include "MiniginPCH.h"
#include "Texture2D.h"
#include <SDL.h>


dae::Texture2D::Texture2D(SDL_Texture* texture)
	:m_Tex{texture}
	, m_Format{}
	, m_Access{}
	, m_Width{}
	, m_Height{}
{
	SDL_QueryTexture(m_Tex, &m_Format, &m_Access, &m_Width, &m_Height);
}

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_Tex);
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_Tex;
}
Uint32 dae::Texture2D::GetFormat() const
{
	return m_Format;
}
int dae::Texture2D::GetAccess() const
{
	return m_Access;
}
int dae::Texture2D::GetWidth() const
{
	return m_Width;
}
int dae::Texture2D::GetHeight() const
{
	return m_Height;
}