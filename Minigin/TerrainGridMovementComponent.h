#pragma once

#include "BaseComponent.h"

#include "EditableTerrainGridComponent.h"

namespace dae
{
	class TerrainGridMovementComponent final : public BaseComponent
	{
	public:
		TerrainGridMovementComponent(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t initPos, float speed,
			bool canCarve = false, float carveSpeed = 0.0f);

		// Only reacts to things when standing still
		void GiveDirection(Direction newDir);
		virtual void Update() override;
		virtual void Initialize() override;
		
	private:
		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		TerrainGridMoveState m_MoveState = TerrainGridMoveState::Still;
		Direction m_Direction = Direction::None;
		size_t m_CurrGridCell = 0;
		size_t m_FutureGridCell = 0;
		bool m_CanCarve = false;
		float m_Speed;
		float m_CarveSpeed;
		bool m_PastHalfCarved = false;
		Float2 m_CenterPos;

		// Private methods
		void SetFutureGridCell();

		void HandleMoveCarve();
		void HandleMoveNoCarve();
	};
}