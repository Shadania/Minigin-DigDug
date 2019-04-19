#pragma once
#include <memory>

#include "TransformComponent.h"
#include "Texture2D.h"
#include "SceneObject.h"


namespace dae
{
	class BaseComponent;
	class TextureComponent;

	/*
	 * GAMEOBJECT:
	 * A container of components
	 * Initializes its own Transform Component
	 */
	class GameObject : public SceneObject, public std::enable_shared_from_this<GameObject>
	{
	public:
		void Update() override;
		void LateUpdate() override;
		void Render() const override;

		void AddComponent(std::shared_ptr<BaseComponent> comp);
		void AddComponentNeedRendering(std::shared_ptr<BaseComponent> comp);
		std::shared_ptr<BaseComponent> GetComponent(const std::string& type);

		std::shared_ptr<dae::TransformComponent> GetTransform();
		void InitTransform(Float2 pos = { 0, 0 }, float rot = 0.0f, Float2 scale = { 1, 1 });

		GameObject();
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::shared_ptr<TransformComponent> m_spTransformComponent;
		std::vector<std::shared_ptr<BaseComponent>> m_ComponentsNeedRendering;
		std::vector<std::shared_ptr<BaseComponent>> m_Components;
	};
}
