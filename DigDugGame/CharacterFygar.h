#pragma once
#include "BaseComponent.h"

namespace dae
{
	class SpriteComponent;

	class CharacterFygar final : public BaseComponent
	{
	public:
		CharacterFygar();

		virtual void Initialize() override;

	private:
		std::shared_ptr<SpriteComponent> m_spSpriteComp;
	};
}