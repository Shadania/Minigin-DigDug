#include "MiniginPCH.h"
#include "SpriteComponent.h"
#include "ServiceLocator.h"
#include "Renderer.h"
#include "GameObject.h"
#include <unordered_map>
#include <algorithm>


#pragma region Sprite
dae::Sprite::Sprite(std::shared_ptr<Texture2D> tex, const std::string& name, size_t amtFrames)
	: m_spTex{ tex }
	, m_Name{ name }
	, m_AmtFrames{amtFrames}
	, m_SecPerFrame{}
	, m_UseUniformTiming{ true }
	, m_AccuSec{0}
	, m_CurrFrameIdx{0}
{}

void dae::Sprite::SetSecPerFrame(float amt)
{
	m_SecPerFrame.clear();
	m_SecPerFrame.push_back(amt);
	m_UseUniformTiming = true;
}
void dae::Sprite::SetSecPerFrame(const std::vector<float> amts)
{
	m_SecPerFrame = amts;
	m_UseUniformTiming = false;
}

void dae::Sprite::Render()
{
	m_spTex->GetSDLTexture()


	// ServiceLocator::GetRenderer()->RenderTexture(m_spTex->GetSDLTexture(), destrect srcrect);
}
void dae::Sprite::Update()
{
	m_AccuSec += ServiceLocator::GetGameTime()->GetDeltaT();

	float maxSec{ m_SecPerFrame.front() };
	if (!m_UseUniformTiming)
		maxSec = m_SecPerFrame[m_CurrFrameIdx];

	if (m_AccuSec >= maxSec)
	{
		m_AccuSec -= maxSec;
		m_CurrFrameIdx = (m_CurrFrameIdx + 1) % m_AmtFrames;
	}
}
bool dae::Sprite::IsName(const std::string& name)
{
	std::hash<std::string> hasher;
	return (hasher(name) == hasher(m_Name));
}
#pragma endregion Sprite



#pragma region SpriteComponent
dae::SpriteComponent::SpriteComponent(std::shared_ptr<Texture2D> tex)
	:BaseComponent("SpriteComponent")
{

}

void dae::SpriteComponent::AddSprite(std::shared_ptr<Sprite> sprite)
{
	m_Sprites.push_back(sprite);
}
void dae::SpriteComponent::RemoveSprite(const std::string& name)
{
	for (auto sprite : m_Sprites)
	{
		if (sprite->IsName(name))
		{
			m_Sprites.erase(std::remove(m_Sprites.begin(), m_Sprites.end(), sprite), m_Sprites.end());
			return;
		}
	}
}
void dae::SpriteComponent::SetActiveSprite(const std::string& name)
{
	for (auto sprite : m_Sprites)
	{
		if (sprite->IsName(name))
		{
			m_ActiveSprite = sprite;
			return;
		}
	}
}
void dae::SpriteComponent::Update()
{
	m_ActiveSprite->Update();
}
void dae::SpriteComponent::Render()
{
	m_ActiveSprite->Render();
	// 
}
#pragma endregion SpriteComponent