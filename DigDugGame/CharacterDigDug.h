#pragma once
#include "BaseComponent.h"
#include "TerrainGridMovementComponent.h"

namespace dae
{
	class SpriteComponent;
	class EditableTerrainGridComponent;
	class CollisionComponent;
	class IngameScene;
	class DigDugPump;


	class CharacterDigDug final : public BaseComponent
	{
	public:
		CharacterDigDug(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t startingPos, IngameScene* pScene, size_t idx);


		void HandleCollision();
		
		void RespawnAtCellIdx(size_t idx);

		size_t GetCurrGridIdx() const { return m_spAgent->GetCurrCellIdx(); };
		size_t GetCurrPlayerIdx() const { return m_PlayerIdx; }
		

		void PumpHitSomething();
		void PumpHitNothing();

	protected:
		virtual void Update() override;

	private:
		void Initialize();
		const float m_Speed;
		bool m_IsInitialized = false;

		std::shared_ptr<GameObject> m_spSpriteObject;
		std::shared_ptr<SpriteComponent> m_spSpriteComp;
		std::shared_ptr<TerrainGridMovementComponent> m_spAgent;
		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		std::shared_ptr<CollisionComponent> m_spCollComp;
		std::weak_ptr<DigDugPump> m_wpPump;
		std::string m_CurrSequence = "Right";
		TerrainGridMoveState m_CurrState = TerrainGridMoveState::Still;

		size_t m_StartingPos;
		IngameScene* m_pScene;
		bool m_Dying = false;
		float m_AccuDyingTime = 0.0f;
		const float m_MaxDyingTime = 1.2f;
		Direction m_Direction = Direction::Right;
		bool m_Shooting = false;
		bool m_GotEnemy = false;
		size_t m_PlayerIdx;

		void HandleMovement();
		void HandleShooting();

		Float2 GetMovementFromKeyboard();
		Float2 GetMovementFromController();
	};
}