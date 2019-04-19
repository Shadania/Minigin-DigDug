#include "MiniginPCH.h"
#include "SpriteComponent.h"
#include "ServiceLocator.h"
#include "Renderer.h"
#include "GameObject.h"
#include <unordered_map>
#include <algorithm>


#pragma region Sprite
dae::Sequence::Sequence(std::shared_ptr<Texture2D> tex, const std::string& name, size_t amtFrames)
	: m_spTex{ tex }
	, m_Name{ name }
	, m_NameHash{}
	, m_AmtFrames{amtFrames}
	, m_SecPerFrame{}
	, m_UseUniformTiming{ true }
	, m_AccuSec{0}
	, m_CurrFrameIdx{0}
{
	std::hash<std::string> hasher;
	m_NameHash = hasher(m_Name);
}

void dae::Sequence::SetSecPerFrame(float amt)
{
	m_SecPerFrame.clear();
	m_SecPerFrame.push_back(amt);
	m_UseUniformTiming = true;
}
void dae::Sequence::SetSecPerFrame(const std::vector<float> amts)
{
	m_SecPerFrame = amts;
	m_UseUniformTiming = false;
}

void dae::Sequence::Render(const std::shared_ptr<GameObject>& go) const
{
	Float4 destRect{}, srcRect{};
	destRect.z = (float)(m_spTex->GetWidth() / (int)m_AmtFrames);
	destRect.w = (float)m_spTex->GetHeight();
	auto pos{go->GetTransform()->GetPos()};
	destRect.x = pos.x - (destRect.z / 2);
	destRect.y = pos.y - (destRect.w / 2);

	srcRect.x = destRect.z * m_CurrFrameIdx;
	srcRect.y = 0;
	srcRect.z = destRect.z;
	srcRect.w = destRect.w;

	ServiceLocator::GetRenderer()->RenderTexture(*m_spTex, destRect, srcRect);
}
void dae::Sequence::Update()
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
bool dae::Sequence::IsName(int nameHash)
{
	return (nameHash == m_NameHash);
}
#pragma endregion Sprite



#pragma region SpriteComponent
dae::SpriteComponent::SpriteComponent()
	:BaseComponent("SpriteComponent")
{

}

void dae::SpriteComponent::AddSprite(std::shared_ptr<Sequence> sprite)
{
	m_Sprites.push_back(sprite);
}
void dae::SpriteComponent::RemoveSprite(const std::string& name)
{
	int nameHash{};
	std::hash<std::string> hasher;
	nameHash = hasher(name);

	for (auto sprite : m_Sprites)
	{
		if (sprite->IsName(nameHash))
		{
			m_Sprites.erase(std::remove(m_Sprites.begin(), m_Sprites.end(), sprite), m_Sprites.end());
			return;
		}
	}
}
void dae::SpriteComponent::SetActiveSprite(const std::string& name)
{
	int nameHash{};
	std::hash<std::string> hasher;
	nameHash = hasher(name);
	for (auto sprite : m_Sprites)
	{
		if (sprite->IsName(nameHash))
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
void dae::SpriteComponent::Render() const
{
	m_ActiveSprite->Render(m_spMyObj.lock());
}
#pragma endregion SpriteComponent