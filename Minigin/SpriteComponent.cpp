#include "MiniginPCH.h"
#include "SpriteComponent.h"
#include "ServiceLocator.h"
#include "Renderer.h"
#include "GameObject.h"
#include <unordered_map>
#include <algorithm>


#pragma region Sequence
dae::Sequence::Sequence(std::shared_ptr<Texture2D> tex, 
	const std::string& name, size_t amtFrames,
	bool resetDeltaOnActive, float secPerFrame)
	: m_spTex{ tex }
	, m_Name{ name }
	, m_NameHash{}
	, m_AmtFrames{amtFrames}
	, m_SecPerFrame{secPerFrame}
	, m_CurrFrameIdx{0}
	,m_ResetDeltaOnActive{resetDeltaOnActive}
{
	std::hash<std::string> hasher;
	m_NameHash = hasher(m_Name);
}

void dae::Sequence::SetSecPerFrame(float amt)
{
	m_SecPerFrame.clear();
	m_SecPerFrame.push_back(amt);
}
void dae::Sequence::SetSecPerFrame(const std::vector<float> amts)
{
	m_SecPerFrame = amts;
}

void dae::Sequence::Render(const std::shared_ptr<GameObject>& go) const
{
	Float4 destRect{}, srcRect{};
	destRect.z = (float)(m_spTex->GetWidth() / (int)m_AmtFrames);
	destRect.w = (float)m_spTex->GetHeight();
	auto pos{go->GetTransform()->GetWorldPos()};
	destRect.x = pos.x - (destRect.z / 2);
	destRect.y = pos.y - (destRect.w / 2);

	srcRect.x = destRect.z * m_CurrFrameIdx;
	srcRect.y = 0;
	srcRect.z = destRect.z;
	srcRect.w = destRect.w;

	ServiceLocator::GetRenderer()->RenderTexture(*m_spTex, destRect, srcRect);
}
void dae::Sequence::Update(float& accuSec)
{
	if (m_SecPerFrame.front() < 0)
		return;

	float maxSec{ m_SecPerFrame.front() };
	if (m_SecPerFrame.size() > 1)
		maxSec = m_SecPerFrame[m_CurrFrameIdx];

	if (accuSec >= maxSec)
	{
		accuSec -= maxSec;
		m_CurrFrameIdx = (m_CurrFrameIdx + 1) % m_AmtFrames;
	}
}
bool dae::Sequence::IsName(size_t nameHash)
{
	return (nameHash == m_NameHash);
}
void dae::Sequence::Freeze()
{
	m_CurrFrameIdx = 0;
}
#pragma endregion Sequence



#pragma region SpriteComponent
dae::SpriteComponent::SpriteComponent()
	:BaseComponent("SpriteComponent")
	,m_AccuSec{0.0f}
	,m_Frozen{false}
{

}

void dae::SpriteComponent::AddSequence(std::shared_ptr<Sequence> sprite)
{
	m_Sprites.push_back(sprite);
}
void dae::SpriteComponent::RemoveSprite(const std::string& name)
{
	std::hash<std::string> hasher;
	size_t nameHash{ hasher(name) };

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
	std::hash<std::string> hasher;
	size_t nameHash{ hasher(name) };
	if (m_ActiveSprite)
		if (m_ActiveSprite->IsName(nameHash))
			return;

	for (auto sprite : m_Sprites)
	{
		if (sprite->IsName(nameHash))
		{
			if (sprite->m_ResetDeltaOnActive)
				m_AccuSec = 0.0f;

			m_ActiveSprite = sprite;
			return;
		}
	}
}
bool dae::SpriteComponent::IsActiveSprite(const std::string& name) const
{
	std::hash<std::string> hasher;
	size_t nameHash{ hasher(name) };
	return m_ActiveSprite->IsName(nameHash);
}

void dae::SpriteComponent::Update()
{
	if (!m_Frozen)
		m_AccuSec += ServiceLocator::GetGameTime()->GetDeltaT();

	m_ActiveSprite->Update(m_AccuSec);
}
void dae::SpriteComponent::Render() const
{
	m_ActiveSprite->Render(m_wpMyObj.lock());
}

void dae::SpriteComponent::Freeze(bool resetDelta)
{
	m_Frozen = true;
	if (resetDelta)
		m_AccuSec = 0.0f;
	m_ActiveSprite->Freeze();
}
void dae::SpriteComponent::Unfreeze()
{
	m_Frozen = false;
}
#pragma endregion SpriteComponent