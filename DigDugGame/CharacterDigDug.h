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
		CharacterDigDug(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, const Float2& startingPos);
	
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
		MoveInfo m_CurrState = MoveInfo::Stopped;
		Direction m_PrevDir = Direction::None;

		Float2 m_StartingPos;
	};
}