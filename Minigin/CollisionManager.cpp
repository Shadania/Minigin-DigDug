#include "MiniginPCH.h"
#include "CollisionManager.h"
#include "CollisionComponent.h"
#include "utils.h"


void dae::CollisionManager::CalculateCollision()
{
	for (size_t i{}; i < m_vpObjs.size(); ++i)
	{
		auto& obj1 = m_vpObjs[i];
		if (!obj1->m_ReceiveCollision)
			continue;
		for (size_t j{}; j < m_vpObjs.size(); ++j)
		{
			auto& obj2 = m_vpObjs[j];
			if (!obj2->m_SendCollision)
				continue;
			
			if (*obj1 == *obj2)
				continue;

			if (std::find(obj1->m_CollisionTargets.begin(), obj1->m_CollisionTargets.end(), obj2->m_Tag) == obj1->m_CollisionTargets.end())
				continue; // obj1 does not want to collide with obj2

			if (std::find(obj2->m_DontGenerateFor.begin(), obj2->m_DontGenerateFor.end(), obj1->m_Tag) != obj2->m_DontGenerateFor.end())
				continue; // obj2 does not want to generate for obj1

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
dae::CollisionComponent* dae::CollisionManager::ImmediateCollision(const Float4& shape, const std::vector<size_t>& targets)
{
	for (size_t j{}; j < m_vpObjs.size(); ++j)
	{
		auto& other = m_vpObjs[j];
		if (!other->m_SendCollision) // if other object does not want to send collision, it will not send collision
			continue;

		if (std::find(targets.begin(), targets.end(), other->m_Tag) == targets.end())
			continue; // obj1 does not want to collide with obj2

		// check if collide
		if (dae::utils::DoRectsOverlap(shape, other->m_Shape))
		{
			// yes collide
			return other;
		}
	}

	return nullptr;
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