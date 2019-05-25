#pragma once
#include "BaseComponent.h"
#include "Events.h"
#include "EditableTerrainGridComponent.h"

namespace dae
{
	class Texture2D;
	class TerrainGridMovementComponent;
	class CollisionComponent;

	class DigDugPump final : public BaseComponent
	{
	public:
		DigDugPump(std::shared_ptr<EditableTerrainGridComponent> spTerrain, Direction dir, size_t initPos, const Float2& startOffset);

		virtual void Initialize() override;
		virtual void Update() override;

		std::shared_ptr<Event> m_spHitCollision;
		std::shared_ptr<Event> m_spHitTerrain;

		static void InitRes();

		void HandleCollision();

	private:
		friend class CharacterDigDug;

		static bool m_ResourcesInitialized;
		static std::shared_ptr<Texture2D> m_spWireHeadBottom;
		static std::shared_ptr<Texture2D> m_spWireHeadTop;
		static std::shared_ptr<Texture2D> m_spWireHeadLeft;
		static std::shared_ptr<Texture2D> m_spWireHeadRight;
		static std::shared_ptr<Texture2D> m_spWireHorizontal;
		static std::shared_ptr<Texture2D> m_spWireVertical;

		std::shared_ptr<EditableTerrainGridComponent> m_spTerrain;
		std::shared_ptr<TerrainGridMovementComponent> m_spAgent;
		std::shared_ptr<CollisionComponent> m_spCollComp;
		std::shared_ptr<GameObject> m_spPumpHead;

		const size_t m_StartingPos;
		const Direction m_Dir;
		const float m_Speed{ 80.0f };
		float m_CurrOffset{0.0f};
		const float m_ImgSize{ 16.0f }; // the images used for the pump are 16 pixels
		size_t m_AmtLinks{ 0 };
		Float2 m_StartOffset;

		void AddLink();
	};
}