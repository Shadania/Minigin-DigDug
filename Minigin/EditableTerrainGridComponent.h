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
			// To stop weirdly going up or down when going left or right and the other way around by mis calculating the position
			pos.x += *m_pWidth / 16;
			pos.y += *m_pHeight / 16;

			// x check
			if ((pos.x >= m_BotLeft.x) && (pos.x <= (m_BotLeft.x + *m_pWidth)))
			{
				// y check
				if ((pos.y >= m_BotLeft.y) && (pos.y <= (m_BotLeft.y + *m_pHeight)))
				{
					return true;
				}
			}
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
	};

	class EditableTerrainGridComponent final : public BaseComponent
	{
	public:
		EditableTerrainGridComponent(float cellHeight, float cellWidth, size_t amtCols, size_t amtRows, const std::string& tileFile);
		virtual ~EditableTerrainGridComponent();

		bool DoesCollide(Float4& shape);
		void SetOffset(const Float2& offset);

		void Carve(const Float4& shape);
		void CanMoveInto(const Float4& shape, const Float2& direction);
		TerrainCell* GetCellAtPos(const Float2& pos) const;
		float GetCellWidth() const { return m_CellWidth; }
		float GetCellHeight() const { return m_CellHeight; }

		Float4& GetBoundaries() { return m_Boundaries; }

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

		std::shared_ptr<Texture2D> m_pTileTex;

		Float4 m_Boundaries = {};

		// goes from left bottom row per row until right top
		TerrainCell* m_pCells;
		// std::vector<TerrainCell> m_Cells;
		
		// PRIVATE METHODS
		void GetCellsOverlappingWith(Float4 shape,
			size_t& leftBotCell, size_t& topRightCell,
			size_t& amtCols, size_t& amtRows);
	};
}