#include "MiniginPCH.h"
#include "TransformComponent.h"
#include "GameObject.h"


dae::TransformComponent::TransformComponent(std::shared_ptr<GameObject> pObj)
	:BaseComponent(pObj)
{
	m_Type = "TransformComponent";
}