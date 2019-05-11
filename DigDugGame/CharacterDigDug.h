#pragma once
#include "GameObject.h"
namespace dae
{
	class SpriteComponent;

	class CharacterDigDug final : public GameObject
	{
	public:
		CharacterDigDug();
	
	protected:
		virtual void Update() override;

	private:
		void Initialize();
		const float m_Speed;

		std::shared_ptr<GameObject> m_spSpriteObject;
		std::shared_ptr<SpriteComponent> m_spSpriteObjectComponent;
		std::string m_CurrSequence;
	};
}