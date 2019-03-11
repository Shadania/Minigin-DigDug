#pragma once
#include "GameObject.h"

namespace dae
{
	class BaseComponent
	{
	public:
		BaseComponent(GameObject* pObj);
		virtual ~BaseComponent() = default;
		virtual void Update() = 0;

		GameObject* GameObj() const;

	private:
		GameObject* m_MyObj;
	};
}