#pragma once
#include "BaseComponent.h"

namespace dae
{
	class Texture2D;

	class TerrainCell
	{
	public:
		TerrainCell() = default;
		~TerrainCell() = default;

		void Init(float x, float y)
		{
			m_BotLeft.x = x;
			m_BotLeft.y = y;
			m_Active = true;
		}

		bool PointInCell(Float2 pos)
		{
			float x{ (pos.x + m_pOffset->x )- m_BotLeft.x }, y{ (pos.y + m_pOffset->y) - m_BotLeft.y };
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

		static void SetWidthHeightOffset(float* newWidth, float* newHeight, Float2* newOffset)
		{
			m_pWidth = newWidth;
			m_pHeight = newHeight;
			m_pOffset = newOffset;
		}

		USHORT GetColorID() const
		{
			return m_ColorID;
		}
		Float2 GetBotLeft() const
		{
			return m_BotLeft;
		}
	private:
		Float2 m_BotLeft = {-1, -1};
		static float* m_pWidth;
		static float* m_pHeight;
		static Float2* m_pOffset;
		bool m_Active = false;
		USHORT m_ColorID = 1;
	};

	class EditableTerrainGridComponent final : public BaseComponent
	{
	public:
		EditableTerrainGridComponent(float cellHeight, float cellWidth, size_t amtCols, size_t amtRows, const std::string& tileFile);
		virtual ~EditableTerrainGridComponent();

		bool DoesCollide(Float4& shape);
		void EraseTerrain(const Float4& shape);
		// this component will take over management of the Colors heap data
		void SetColors(int amtColors, Float3* colors);
		void SetOffset(const Float2& offset);

		void Render() const override;
		virtual void Initialize() override;

	protected:


	private:
		// DATA MEMBERS
		float m_CellHeight;
		float m_CellWidth;
		size_t m_AmtCols;
		size_t m_AmtRows;
		size_t m_AmtCells;
		Float2 m_Offset = { 0, 0 };

		Float3* m_pColors;
		int m_AmtColors;
		std::shared_ptr<Texture2D> m_pTileTex;

		// goes from left bottom row per row until right top
		TerrainCell* m_pCells;
		// std::vector<TerrainCell> m_Cells;
		
		// PRIVATE METHODS
		void GetCellsOverlappingWith(Float4 shape,
			size_t& leftBotCell, size_t& topRightCell,
			size_t& amtCols, size_t& amtRows);
	};
}