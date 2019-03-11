#pragma once
#include <memory>

#include "Transform.h"
#include "Texture2D.h"
#include "SceneObject.h"


namespace dae
{
	class BaseComponent;

	class GameObject final : public SceneObject
	{
	public:
		void Update() override;
		void LateUpdate() override;
		void Render() const override;

		void SetTexture(const std::string& filename);
		void SetPosition(float x, float y);

		void AddComponent(BaseComponent*comp);


		GameObject() = default;
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		Transform m_Transform;
		std::shared_ptr<Texture2D> m_Texture;
		std::vector<BaseComponent*> m_Components;
	};
}
