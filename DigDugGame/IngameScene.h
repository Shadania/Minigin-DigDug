#pragma once
#include "Scene.h"

namespace dae
{
	class CharacterDigDug;

	class IngameScene final : public Scene
	{
	public:
		IngameScene();

		virtual void Init() override;
		virtual void Update() override;

	private:
		float m_Scale = 2.0f;
	};
}


