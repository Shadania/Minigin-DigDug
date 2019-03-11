#pragma once
#include "BaseComponent.h"

namespace dae
{
	class RenderComponent final : public BaseComponent
	{
	public:
		RenderComponent(GameObject* pObj);

		virtual void Update();
	private:

		Texture2D* m_Texture;
	};
}