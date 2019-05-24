#pragma once
// Mildly inspired on what I heard from Dylan Millian about his collision system, namely using tags

#include "BaseComponent.h"
#include "Events.h"

namespace dae
{
	class CollisionComponent final : public BaseComponent
	{
	public:
		CollisionComponent(size_t tag);


		virtual void Initialize() override;
		virtual void OnDestroy() override;

		void SetCollTargets(std::vector<size_t> targs);
		void AddCollTarget(size_t targ);
		void RemoveCollTarget(size_t targ);

		void SetShape(const Float4& shape) { m_Shape = shape; }

		Event m_HasCollided;
		size_t GetCollidedTag() { return m_pCollidedObj->m_Tag; }
		CollisionComponent* GetCollidedObj() { return m_pCollidedObj; }

		bool operator==(const CollisionComponent& other)
		{
			// ID of an initialized component is always 1 or more!!
			if (m_Id == 0 || other.m_Id == 0)
				return false;

			return m_Id == other.m_Id;
		}

	private:
		friend class CollisionManager;

		size_t m_Tag;
		size_t m_Id = 0;
		std::vector<size_t> m_CollisionTargets;
		Float4 m_Shape;
		CollisionComponent* m_pCollidedObj;
	};
}