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


void dae::CollisionComponent::Initialize()
{
	ServiceLocator::GetCollisionManager()->AddObject(this);
}
void dae::CollisionComponent::OnDestroy()
{
	ServiceLocator::GetCollisionManager()->RemoveObject(this);
}