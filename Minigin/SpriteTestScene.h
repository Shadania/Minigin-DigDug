#pragma once
#include "Scene.h"

namespace dae
{
	class GameObject;

	class SpriteTestScene final : public Scene
	{
	public:
		SpriteTestScene();

		virtual void Init() override;
		virtual void Update() override;

	private:
		std::shared_ptr<GameObject> m_SpriteObject;
	};
}