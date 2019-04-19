#pragma once
#include "Structs.h"
#include "BaseComponent.h"


namespace dae
{
	class BaseComponent;

	class TransformComponent final : public BaseComponent
	{
	public:
		TransformComponent();

		void SetPos(Float2 newPos) { m_Pos = newPos; }
		void SetRotEuler(float newRot) { m_Angle = newRot; }
		void SetScale(Float2 newScale) { m_Scale = newScale; }

		Float2 GetPos() const { return m_Pos; }
		float GetRotEuler() const { return m_Angle; }
		Float2 GetScale() const { return m_Scale; }

		void Translate(float x, float y);
		void Rotate(float angle);
		void Scale(float x, float y);


		TransformComponent(const TransformComponent&) = delete;
		TransformComponent(TransformComponent&&) noexcept = delete;
		TransformComponent& operator=(const TransformComponent&) = delete;
		TransformComponent& operator=(TransformComponent&&) noexcept = delete;

	private:
		Float2 m_Pos;
		float m_Angle;
		Float2 m_Scale;
	};
}