#include "MiniginPCH.h"
#include "TransformComponent.h"
#include "GameObject.h"


dae::TransformComponent::TransformComponent(GameObject* go)
	:BaseComponent("TransformComponent")
	,m_pMyObj{go}
{
	m_Type = "TransformComponent";
}

void dae::TransformComponent::Translate(float x, float y)
{
	m_Pos.x += x;
	m_Pos.y += y;
}
void dae::TransformComponent::Rotate(float angle)
{
	m_Rot += angle;
}
void dae::TransformComponent::Scale(float x, float y)
{
	m_Scale.x += x;
	m_Scale.y += y;
}

dae::Float2 dae::TransformComponent::GetWorldPos() const
{
	Float2 result{m_Pos};
	
	GameObject* parent{ m_pMyObj };
	while (parent)
	{
		result += parent->GetTransform()->GetLocalPos();
		parent = parent->GetParent();
	}

	return result;
}
float dae::TransformComponent::GetWorldRot() const
{
	float result{ m_Rot };

	GameObject* parent{ m_pMyObj->GetParent() };
	while (parent)
	{
		result += parent->GetTransform()->GetLocalRot();
		parent = parent->GetParent();
	}

	return result;
}
dae::Float2 dae::TransformComponent::GetWorldScale() const
{
	Float2 result{ m_Pos };

	GameObject* parent{ m_pMyObj->GetParent() };
	while (parent)
	{
		result *= parent->GetTransform()->GetLocalScale();
		parent = parent->GetParent();
	}

	return result;
}
