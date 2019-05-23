#pragma once
#include "BaseComponent.h"
#include "TerrainGridMovementComponent.h"

namespace dae
{
	class SpriteComponent;
	class EditableTerrainGridComponent;
	class CollisionComponent;
	class IngameScene;



	class CharacterDigDug final : public BaseComponent
	{
	public:
		CharacterDigDug(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startingPos, IngameScene* pScene);

		void HandleCollision();
		
		void RespawnAtCellIdx(size_t idx);



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
		std::shared_ptr<CollisionComponent> m_spCollComp;
		std::string m_CurrSequence;
		TerrainGridMoveState m_CurrState = TerrainGridMoveState::Still;\

		size_t m_StartingPos;
		IngameScene* m_pScene;
		bool m_Dying = false;
		float m_AccuDyingTime = 0.0f;
		const float m_MaxDyingTime = 1.2f;

		void HandleMovement();
	};
}