#pragma once
#include "Scene.h"

namespace dae
{
	class FPSTestScene final : public Scene
	{
	public:
		FPSTestScene();

		virtual void Init() override;
	};
}