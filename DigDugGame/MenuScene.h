#pragma once
#include "Scene.h"

namespace dae
{
	class MenuScene final : public Scene
	{
	public:
		MenuScene();
		virtual void Initialize() override;


		void ClickedSolo();
		void ClickedCoop();
		void ClickedQuit();
	};
}