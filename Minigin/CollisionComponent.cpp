#include "MiniginPCH.h"
#include "CollisionComponent.h"
#include "ServiceLocator.h"
#include "CollisionManager.h"


dae::CollisionComponent::CollisionComponent(size_t tag)
	:BaseComponent("CollisionComponent")
	,m_Tag{tag}
{

}


void dae::CollisionComponent::SetCollTargets(std::vector<size_t> targs)
{
	m_CollisionTargets = targs;
}
void dae::CollisionComponent::AddCollTarget(size_t targ)
{
	// no duplicates pls
	if (std::find(m_CollisionTargets.begin(), m_CollisionTargets.end(), targ) == m_CollisionTargets.end())
		m_CollisionTargets.push_back(targ);
}
void dae::CollisionComponent::RemoveCollTarget(size_t targ)
{
	m_CollisionTargets.erase(std::remove(m_CollisionTargets.begin(), m_CollisionTargets.end(), targ), m_CollisionTargets.end());
}

void dae::CollisionComponent::StopSendingCollisionFor(size_t tag)
{
	if (std::find(m_DontGenerateFor.begin(), m_DontGenerateFor.end(), tag) == m_DontGenerateFor.end())
		m_DontGenerateFor.push_back(tag);
}
void dae::CollisionComponent::ResumeSendingCollisionFor(size_t tag)
{
	m_DontGenerateFor.erase(std::remove(m_DontGenerateFor.begin(), m_DontGenerateFor.end(), tag), m_DontGenerateFor.end());
}

void dae::CollisionComponent::Initialize()
{
	if (m_IsInitialized)
		return;
	m_IsInitialized = true;
	ServiceLocator::GetCollisionManager()->AddObject(this);
}
void dae::CollisionComponent::OnDestroy()
{
	ServiceLocator::GetCollisionManager()->RemoveObject(this);
}