#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"
#include "utils.h"


void dae::CollisionManager::CalculateCollision()
{
	for (size_t i{}; i < m_vpObjs.size(); ++i)
	{
		auto& obj1 = m_vpObjs[i];
		for (size_t j{}; j < m_vpObjs.size(); ++j)
		{
			auto& obj2 = m_vpObjs[j];
			
			if (*obj1 == *obj2)
				continue;

			if (std::find(obj1->m_CollisionTargets.begin(), obj1->m_CollisionTargets.end(), obj2->m_Tag) == obj1->m_CollisionTargets.end())
				continue; // obj1 does not want to collide with obj2

			// check if collide
			if (dae::utils::DoRectsOverlap(obj1->m_Shape, obj2->m_Shape))
			{
				// yes collide
				obj1->m_pCollidedObj = obj2;
				obj1->m_HasCollided.Invoke();
			}
		}
	}
}


void dae::CollisionManager::AddObject(CollisionComponent* obj)
{
	m_vpObjs.push_back(obj);
	m_CurrMaxId++;
	obj->m_Id = m_CurrMaxId;
}
void dae::CollisionManager::RemoveObject(CollisionComponent* obj)
{
	if (!m_vpObjs.empty())
		m_vpObjs.erase(std::remove(m_vpObjs.begin(), m_vpObjs.end(), obj), m_vpObjs.end());
}