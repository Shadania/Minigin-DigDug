#pragma once
#include "BaseComponent.h"

namespace dae
{
	class EditableTerrainGridComponent;

	enum class Direction
	{
		Up,
		Down,
		Left,
		Right,
		None
	};
	enum class MoveInfo
	{
		Started,
		Stopped,
		ChangedDir
	};



	

	class TerrainGridMovementComponent final : public BaseComponent
	{
		struct AgentState
		{
			virtual void UpdateDirection(Direction* dir, MoveInfo info) = 0;
			virtual void Update() = 0;
			TerrainGridMovementComponent* pAgent;
		};

		struct StateStill : public AgentState
		{
			virtual void UpdateDirection(Direction* dir, MoveInfo info) override;
			virtual void Update() override;
		};
		struct StateMoving : public AgentState
		{
			virtual void UpdateDirection(Direction* dir, MoveInfo info) override;
			virtual void Update() override;
		};
		struct StateFinishing : public AgentState
		{
			virtual void UpdateDirection(Direction* dir, MoveInfo info) override;
			virtual void Update() override;
		};
		struct StateChangingDir : public AgentState
		{
			virtual void UpdateDirection(Direction* dir, MoveInfo info) override;
			virtual void Update() override;
		};


	public:
		TerrainGridMovementComponent(std::shared_ptr<EditableTerrainGridComponent> spTerrain, float speed, bool carveIntoTerrain = false);
		virtual ~TerrainGridMovementComponent() = default;

		void GiveDirection(Direction* dir, MoveInfo info);
		void SetDimensions(const Float2& dimensions);
		void SetDimensions(float w, float h);
		void SetPos(const Float2& pos) { m_CurrPos = pos; }

		virtual void Update() override;

	private:
		friend struct AgentState;

		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		Float2 m_CurrPos = {};
		Float2 m_TargetPos = {};
		Float2 m_Dimensions = {};
		Direction m_CurrDir = Direction::Right;
		Direction m_FutureDir = Direction::Right;
		float m_Speed = 0.0f;
		bool m_CarveIntoTerrain = false;
		std::shared_ptr<AgentState> m_pState;

		// PRIVATE METHODS
		void HandleVelocity();
		bool HasArrived();
		void SetTargetPosHere();
		void DoCarve();
		void DoCollision();

		void SetState(const std::shared_ptr<AgentState>& newState);
	};
}

