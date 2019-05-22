#pragma once
#include "BaseComponent.h"
#include "TerrainGridMovementComponent.h"

namespace dae
{
	class SpriteComponent;
	class EditableTerrainGridComponent;




	class CharacterDigDug final : public BaseComponent
	{
	public:
		CharacterDigDug(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startingPos);
	
	protected:
		virtual void Update() override;

	private:
		void Initialize();
		const float m_Speed;
		bool m_IsInitialized = false;

		std::shared_ptr<GameObject> m_spSpriteObject;
		std::shared_ptr<SpriteComponent> m_spSpriteObjectComponent;
		std::shared_ptr<TerrainGridMovementComponent> m_spGridAgentComponent;
		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		std::string m_CurrSequence;
		TerrainGridMoveState m_CurrState = TerrainGridMoveState::Still;\

		size_t m_StartingPos;
	};
}