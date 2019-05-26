#pragma once
#include "BaseComponent.h"
#include <deque>

namespace dae
{
	class Texture2D;
	class EditableTerrainGridComponent;


	enum class Direction
	{
		Up,
		Down,
		Left,
		Right,
		None
	};		
	enum class TerrainGridMoveResult
	{
		Go,
		Blocked,
		Carving
	};
	enum class TerrainGridMoveState
	{
		Still,
		Moving,
		Carving
	};

	class EditableTerrainGridCell
	{
	public:
		enum class Part
		{
			Base,
			Top,
			Bottom,
			Left,
			Right
		};
		struct DugState
		{
			bool m_DugBase = false;

			bool m_DugTop = false;
			bool m_DugBottom = false;
			bool m_DugLeft = false;
			bool m_DugRight = false;
		};

		EditableTerrainGridCell() = default;
		EditableTerrainGridCell(const Float2& centerPos, EditableTerrainGridComponent* pTerrain, bool completelyOpen = false, bool blocked = false, DugState dugState = DugState());

		

		void SetPartDug(Part which, bool dug);
		void SetAllDug(DugState state);
		const DugState& GetDugState() const { return m_DugState; }
		void Render() const;

		void SetCenterPos(const Float2& pos) { m_CenterPos = pos; }
		const Float2& GetCenterPos() const { return m_CenterPos; }
		void SetCompletelyOpen(bool completelyOpen) { m_CompletelyOpen = completelyOpen; };
		bool IsCompletelyOpen() const { return m_CompletelyOpen; }
		void SetBlocked(bool blocked) { m_Blocked = blocked; }
		bool IsBlocked() const { return m_Blocked; }
		bool IsPosInCell(const Float2& pos);

	private:

		// STATICS
		static bool m_ResourcesInitialized;

		static std::shared_ptr<Texture2D> m_GroundBase;

		static std::shared_ptr<Texture2D> m_GroundTop;
		static std::shared_ptr<Texture2D> m_GroundBottom;
		static std::shared_ptr<Texture2D> m_GroundLeft;
		static std::shared_ptr<Texture2D> m_GroundRight;

		EditableTerrainGridComponent* m_pTerrain;


		// STATIC PRIVATE METHODS
		static void InitResources();

		// PRIVATE DATA MEMBERS
		DugState m_DugState = {};
		Float2 m_CenterPos = {};
		bool m_CompletelyOpen = false;
		bool m_Blocked = false;
	};


	struct PathfindNode
	{
		PathfindNode(size_t idx, Direction from, std::shared_ptr<PathfindNode> prev)
			: idx{ idx }
			, from{ from }
			, prev{prev}
		{}
		bool operator==(const PathfindNode& other)
		{
			return (idx == other.idx);
		}

		size_t idx;
		Direction from;
		std::shared_ptr<PathfindNode> prev;
	};

	class EditableTerrainGridComponent final : public BaseComponent
	{
	public:
		EditableTerrainGridComponent(size_t rows, size_t cols, const Float2& dims, const std::string& background, const Float2& offset = { 0, 0 } );

		virtual void Initialize() override;
		virtual void Render() const override;



		TerrainGridMoveResult TryGo(Direction dir, size_t from, bool canCarve, const std::vector<size_t>& ignoredCells, bool canGoThroughThinWalls = false);
		bool ContinueCarve(Direction srcDir, size_t target);

		// Pathfinding
		bool GenerateNoCarvePath(std::deque<Direction>& path, size_t src, size_t dest);

		bool CanGoFrom(size_t pos, Direction dir);

		// For setup
		void DirectCarve(size_t idx, EditableTerrainGridCell::DugState which);
		void SetCellCompletelyOpen(size_t idx);

		void SetCellBlocked(size_t idx);
		void SetCellUnblocked(size_t idx);


		

		void SetUseLayers(bool set, size_t layerSize);

		bool IsCellFree(size_t idx);

		const Float2& GetCenterPosOfCellIdx(size_t idx) const;

		size_t AmtRows() const { return m_Rows; }
		size_t AmtCols() const { return m_Cols; }

		float CellWidth() const { return m_CellDims.x; }
		float CellHeight() const { return m_CellDims.y; };

		size_t GetCellAtPos(const Float2& pos);

		size_t GetLayerIdxFromCellIdx(size_t cellIdx);

		Float4 GetCellShapeAtIdx(size_t idx);

	private:
		size_t m_Rows = 0;
		size_t m_Cols = 0;

		const Float2 m_Dims;
		const Float2 m_CellDims;
		const Float2 m_Offset;

		std::vector<EditableTerrainGridCell> m_vCells = {};
		std::shared_ptr<Texture2D> m_spBackground = {};

		bool m_UseLayers = false;
		size_t m_LayerSize = 0;

		// Pathfinding
		std::deque<std::shared_ptr<PathfindNode>> GetPossibleConnections(std::shared_ptr<PathfindNode> from);
	};
}