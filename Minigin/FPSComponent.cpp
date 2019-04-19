#include "MiniginPCH.h"
#include "FPSComponent.h"
#include "ServiceLocator.h"
#include "GameTime.h"


dae::FPSComponent::FPSComponent(const std::shared_ptr<Font> font, const Float4& color)
	:BaseComponent("FPSComponent")
	,m_spText{}
	,m_spFont{font}
	,m_Color{color}
{
	Initialize();
}
void dae::FPSComponent::Initialize()
{
	m_spText = std::make_shared<TextComponent>("FPS: ???", m_spFont, m_Color);
	m_spText->GenerateTexture();
}


void dae::FPSComponent::SetColor(const Float4& newColor)
{
	m_spText->SetColor(newColor);
}
void dae::FPSComponent::SetFont(const std::shared_ptr<Font>& newFont)
{
	m_spText->SetFont(newFont);
}

void dae::FPSComponent::Update()
{
	m_spText->SetText("FPS: " + std::to_string(ServiceLocator::GetGameTime()->GetFPS()));
	m_spText->GenerateTexture();
}

void dae::FPSComponent::Render()
{
	m_spText->Render();
}
void dae::FPSComponent::SetGameObj(std::shared_ptr<GameObject> newObj)
{
	m_spMyObj = std::weak_ptr<GameObject>(newObj);
	m_spText->SetGameObj(newObj);
}