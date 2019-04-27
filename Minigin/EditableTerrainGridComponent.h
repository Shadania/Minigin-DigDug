#pragma once
#include "BaseComponent.h"

namespace dae
{
	class TerrainCell
	{
	public:
		void Init(float x, float y, float* width, float* height)
		{
			m_BotLeft.x = x;
			m_BotLeft.y = y;
			m_pWidth = width;
			m_pHeight = height;
			m_Active = true;
		}

		bool PointInCell(Float2 pos)
		{
			float x{ pos.x - m_BotLeft.x }, y{ pos.y - m_BotLeft.y };
			if (x > 0 && x < *m_pWidth)
				if (y > 0 && y < *m_pHeight)
					return true;
			return false;
		}
		void SetInactive()
		{
			m_Active = false;
		}
		bool IsActive()
		{
			return m_Active;
		}
	private:
		Float2 m_BotLeft;
		float* m_pWidth;
		float* m_pHeight;
		bool m_Active;
	};

	class EditableTerrainGridComponent final : public BaseComponent
	{
	public:
		EditableTerrainGridComponent(float cellHeight, float cellWidth, size_t amtCols, size_t amtRows);
		~EditableTerrainGridComponent();
		
		bool DoesCollide(Float4& shape);
		void EraseTerrain(const Float4& shape);


	private:
		// DATA MEMBERS
		float m_CellHeight;
		float m_CellWidth;
		size_t m_AmtCols;
		size_t m_AmtRows;
		size_t m_AmtCells;

		// goes from left bottom row per row until right top
		TerrainCell* m_pCells;

		
		// PRIVATE METHODS
		void Initialize();
		void GetCellsOverlappingWith(Float4 shape,
			size_t& leftBotCell, size_t& topRightCell,
			size_t& amtCols, size_t& amtRows);
	};
}