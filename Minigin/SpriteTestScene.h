#pragma once
#include "Scene.h"

namespace dae
{
	class SpriteTestScene final : public Scene
	{
	public:
		SpriteTestScene();

		virtual void Init() override;
	};
}