#pragma once
#include <string>

namespace dae
{
	class GameObject;
	class TransformComponent;

	class BaseComponent
	{
	public:
		BaseComponent(const std::string& type = "BaseComponent");
		virtual ~BaseComponent() = default;
		virtual void FixedUpdate() {}
		virtual void Update() {}
		virtual void LateUpdate() {}
		virtual void Render() const {}
		virtual void Initialize() {}
		virtual void OnDestroy() {}

		std::shared_ptr<dae::GameObject> GameObj() const;
		virtual void SetGameObj(std::shared_ptr<GameObject> newObj);
		std::string GetType() const;

		BaseComponent(const BaseComponent&) = delete;
		BaseComponent(BaseComponent&&) noexcept = delete;
		BaseComponent& operator=(const BaseComponent&) = delete;
		BaseComponent& operator=(BaseComponent&&) noexcept = delete;

		
		// GameObject functions rerouted through here for ease of access
		void AddChild(std::shared_ptr<GameObject> gObj);
		std::shared_ptr<TransformComponent> GetTransform();
		void AddComponent(std::shared_ptr<BaseComponent> comp);
		void AddComponentNeedRendering(std::shared_ptr<BaseComponent> comp);
		void Destroy();

	protected:
		std::weak_ptr<GameObject> m_wpMyObj;
		std::string m_Type;
		bool m_IsInitialized = false;
	};
}