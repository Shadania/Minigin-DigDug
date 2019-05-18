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
		std::shared_ptr<CharacterDigDug> m_spCharacter;
	};
}


