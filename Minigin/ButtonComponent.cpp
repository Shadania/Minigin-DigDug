#include "MiniginPCH.h"
#include "ButtonComponent.h"

#include "InputManager.h"
#include "ServiceLocator.h"
#include "ResourceManager.h"
#include "TextureComponents.h"
#include "Renderer.h"
#include "Events.h"
#include "utils.h"
#include "TransformComponent.h"


dae::ButtonComponent::ButtonComponent(const std::shared_ptr<Texture2D> tex, const Float2& dims )
	:BaseComponent("ButtonComponent")
	,m_spMyTexture{tex}
	,m_Dims{dims}
{}


void dae::ButtonComponent::Initialize()
{
	m_spGotClicked = std::make_shared<Event>();

	auto input = ServiceLocator::GetInputManager();
	auto list = std::make_shared<Listener>();
	list->SetFunction([this]() {this->MouseClicked(); });
	input->m_spMouseUp->AddListener(list);

	m_spText = std::make_shared<TextComponent>("ButtonText", ServiceLocator::GetResourceManager()->GetDefaultFont());
	m_spText->GenerateTexture();
}
void dae::ButtonComponent::MouseClicked()
{
	auto input = ServiceLocator::GetInputManager();
	Float2 pos{ GetTransform()->GetWorldPos() };
	Float2 mousePos{ input->GetMousePos() };
	mousePos /= ServiceLocator::GetRenderer()->GetScale();
	if (utils::IsPointInRect(Float4(pos, m_Dims), mousePos))
	{
		m_spGotClicked->Invoke();
	}
}


void dae::ButtonComponent::SetTexture(const std::shared_ptr<Texture2D> tex)
{
	m_spMyTexture = tex;
}
void dae::ButtonComponent::SetTexture(const std::string& asset)
{
	m_spMyTexture = ServiceLocator::GetResourceManager()->LoadTexture(asset);
}
void dae::ButtonComponent::SetText(const std::string& text)
{
	m_spText->SetText(text);
	m_spText->GenerateTexture();
}
void dae::ButtonComponent::SetFont(const std::shared_ptr<Font>& font)
{
	m_spText->SetFont(font);
	m_spText->GenerateTexture();
}

void dae::ButtonComponent::Render() const
{
	Float2 pos{GetTransform()->GetWorldPos()};
	ServiceLocator::GetRenderer()->RenderTexture(*m_spMyTexture, Float4(pos, m_Dims));
	if (m_UseText)
		ServiceLocator::GetRenderer()->RenderTexture(*(m_spText->GetTexture()), Float4(pos, m_Dims));
}