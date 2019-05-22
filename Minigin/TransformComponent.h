#pragma once
#include "Structs.h"
#include "BaseComponent.h"


namespace dae
{
	class BaseComponent;

	class TransformComponent final : public BaseComponent
	{
	public:
		TransformComponent(GameObject* go);

		void SetLocalPos(Float2 newPos) { m_Pos = newPos; }
		void SetLocalRot(float newRot) { m_Rot = newRot; }
		void SetLocalScale(Float2 newScale) { m_Scale = newScale; }

		Float2 GetLocalPos() const { return m_Pos; }
		float GetLocalRot() const { return m_Rot; }
		Float2 GetLocalScale() const { return m_Scale; }

		Float2 GetWorldPos() const;
		float GetWorldRot() const;
		Float2 GetWorldScale() const;

		void Translate(float x, float y);
		void Translate(const Float2& pos);
		void Rotate(float angle);
		void Scale(float x, float y);
		void Scale(const Float2& scale);
		void Scale(float uniformScale);


		TransformComponent(const TransformComponent&) = delete;
		TransformComponent(TransformComponent&&) noexcept = delete;
		TransformComponent& operator=(const TransformComponent&) = delete;
		TransformComponent& operator=(TransformComponent&&) noexcept = delete;

	private:
		Float2 m_Pos;
		float m_Rot;
		Float2 m_Scale;
		GameObject* m_pMyObj;
	};
}