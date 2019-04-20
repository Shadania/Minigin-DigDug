#include "MiniginPCH.h"
#include "TextureComponents.h"
#include "ResourceManager.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "Renderer.h"
#include "Font.h"
#include "GameObject.h"
#include "ServiceLocator.h"


#pragma region TextureComponent

dae::TextureComponent::TextureComponent(Float2 pos, float rot, Float2 scale)
	:BaseComponent("TextureComponent")
	, m_Pos{pos}
	, m_Rot{rot}
	, m_Scale{scale}
{
}

void dae::TextureComponent::SetTexture(const std::string& fileName)
{
	m_spTexture = ServiceLocator::GetResourceManager()->LoadTexture(fileName);
}

void dae::TextureComponent::Render() const
{
	Float2 finalPos{ m_spMyObj.lock()->GetTransform()->GetPos() };
	finalPos.x += m_Pos.x;
	finalPos.y += m_Pos.y;
	ServiceLocator::GetRenderer()->RenderTexture(*m_spTexture, finalPos.x, finalPos.y);
}
#pragma endregion TextureComponent



#pragma region TextComponent
dae::TextComponent::TextComponent(const std::string& text, const std::shared_ptr<Font> font, 
		Float4 color, Float2 pos, float rot, Float2 scale)
	:TextureComponent(pos, rot, scale )
	,m_Text{text}
	,m_spFont{font}
	,m_Color{color}
{
	m_Type = "TextComponent";
}

void dae::TextComponent::SetText(const std::string& newText)
{
	m_Text = newText;
}
void dae::TextComponent::SetFont(const std::shared_ptr<Font>& newFont) 
{
	m_spFont = newFont;
}

void dae::TextComponent::Update()
{

}

void dae::TextComponent::GenerateTexture()
{
	const SDL_Color color = { Uint8(m_Color.x*255.0f), Uint8(m_Color.y * 255.0f), Uint8(m_Color.z*255.0f) };
	const auto surf = TTF_RenderText_Blended(m_spFont->GetFont(), m_Text.c_str(), color);
	if (surf == nullptr)
	{
		throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
	}
	auto texture = SDL_CreateTextureFromSurface(ServiceLocator::GetRenderer()->GetSDLRenderer(), surf);
	if (texture == nullptr)
	{
		throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
	}
	SDL_FreeSurface(surf);
	m_spTexture = std::make_shared<Texture2D>(texture);
}
void dae::TextComponent::SetColor(const Float4& newColor)
{
	m_Color = newColor;
}
#pragma endregion TextComponent