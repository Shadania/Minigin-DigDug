#pragma once

namespace dae
{
	class CollisionComponent;

	class CollisionManager
	{
	public:

		void CalculateCollision();
		CollisionComponent* ImmediateCollision(const Float4& shape, const std::vector<size_t>& targets); // do collision once

		void AddObject(CollisionComponent* obj);
		void RemoveObject(CollisionComponent* obj);
		
	private:
		std::vector<CollisionComponent*> m_vpObjs = {};
		size_t m_CurrMaxId = 0;
	};
}