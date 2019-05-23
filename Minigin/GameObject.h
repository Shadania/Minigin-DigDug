#pragma once
#include <memory>

#include "TransformComponent.h"
#include "Texture2D.h"
#include "SceneObject.h"
#include <vector>


namespace dae
{
	class BaseComponent;
	class TextureComponent;
	class Scene;

	/*
	 * GAMEOBJECT:
	 * A container of components
	 * Initializes its own Transform Component
	 */
	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		void RootFixedUpdate();
		void RootUpdate();
		void RootLateUpdate();
		void RootRender() const;
		void RootInitialize();
		void RootOnDestroy();

		void AddChild(std::shared_ptr<GameObject> child);
		void SetParent(GameObject* parent);
		GameObject* GetParent();
		void AddComponent(std::shared_ptr<BaseComponent> comp);
		void AddComponentNeedRendering(std::shared_ptr<BaseComponent> comp);
		std::shared_ptr<BaseComponent> GetComponent(const std::string& type);

		std::shared_ptr<dae::TransformComponent> GetTransform();

		void Destroy();

		GameObject(const Float2& pos = { 0, 0 }, float rot = 0.0f, const Float2& scale = { 1, 1 });
		virtual ~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render() const;
		void Initialize();
		void OnDestroy();


		friend class Scene;
		std::shared_ptr<TransformComponent> m_spTransformComponent;
		std::vector<std::shared_ptr<BaseComponent>> m_vspComponentsNeedRendering;
		std::vector<std::shared_ptr<BaseComponent>> m_vspComponents;
		std::vector<std::shared_ptr<GameObject>> m_vspChildren;
		GameObject* m_pParent = nullptr;
		Scene* m_pScene = nullptr;

		bool m_IsInitialized = false;
		bool m_IsBeingDestroyed = false;

		void RemoveChild(std::shared_ptr<GameObject> child);
	};
}
