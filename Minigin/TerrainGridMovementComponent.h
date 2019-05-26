#pragma once

#include "BaseComponent.h"

#include "EditableTerrainGridComponent.h"

#include <algorithm>

#include <deque>

namespace dae
{
	class TerrainGridMovementComponent final : public BaseComponent
	{
	public:
		TerrainGridMovementComponent(const std::shared_ptr<EditableTerrainGridComponent>& spTerrain, size_t initPos, float speed,
			bool canCarve = false, float carveSpeed = 0.0f, bool passThroughThinWalls = false);

		// Only reacts to things when standing still
		TerrainGridMoveResult GiveDirection(Direction newDir);
		virtual void Update() override;
		virtual void Initialize() override;

		void AddIdxToIgnoreList(size_t idx) { m_IgnoredCellIdxs.push_back(idx); };
		void RemoveIdxFromIgnoreList(size_t idx) { m_IgnoredCellIdxs.erase(std::remove(m_IgnoredCellIdxs.begin(), m_IgnoredCellIdxs.end(), idx), m_IgnoredCellIdxs.end()); }
		
		const Direction& GetCurrDir()const { return m_Direction; }

		bool FindPathTo(size_t targetIdx);

		size_t GetCurrCellIdx() { return m_CurrGridCell; }
		Float2 GetCurrCenterPos() { return m_CenterPos; }

		void Reset(size_t newPos);
		void Freeze(); // Stops all movement
		void Unfreeze();

		std::vector<Direction> GetPossibleDirections();

		void SetGhostSpeed(float speed) { m_GhostSpeed = speed; }
		void SetGhost(bool ghost, size_t target) { m_IsGhost = ghost; m_GhostTarget = target; }
		bool GetGhost() { return m_IsGhost; }
		float GetSpeed() { return m_Speed; }

	private:
		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		TerrainGridMoveState m_MoveState = TerrainGridMoveState::Still;
		std::vector<size_t> m_IgnoredCellIdxs = {};
		Direction m_Direction = Direction::None;
		size_t m_CurrGridCell = 0;
		size_t m_FutureGridCell = 0;
		bool m_CanCarve = false;
		float m_Speed;
		float m_CarveSpeed;
		bool m_PastHalfCarved = false;
		Float2 m_CenterPos;
		bool m_IsStopped = false;
		bool m_CanGoThroughThinWalls;
		bool m_IsGhost = false;
		float m_GhostSpeed = 0.0f;
		size_t m_GhostTarget = 0; 

		bool m_FollowingPath = false;
		std::deque<Direction> m_CurrentPath = {};

		// Private methods
		void HandleMoveCarve();
		void HandleMoveNoCarve();
		void HandleMoveGhost();
	};
}