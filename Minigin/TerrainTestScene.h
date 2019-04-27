#pragma once
#include "Scene.h"

namespace dae
{
	class CharacterDigDug;

	class TerrainTestScene final : public Scene
	{
	public:
		TerrainTestScene();

		virtual void Init() override;
		virtual void Update() override;

	private:
		std::shared_ptr<CharacterDigDug> m_spCharacter;
	};
}