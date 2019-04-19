#include "MiniginPCH.h"
#include "TransformComponent.h"
#include "GameObject.h"


dae::TransformComponent::TransformComponent()
	:BaseComponent()
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
	m_Angle += angle;
}
void dae::TransformComponent::Scale(float x, float y)
{
	m_Scale.x += x;
	m_Scale.y += y;
}