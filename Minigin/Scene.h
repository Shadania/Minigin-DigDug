#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;

	class Scene
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void AddToScene(const std::shared_ptr<GameObject>& object);

		virtual void FixedUpdate();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render() const;

		virtual void Init() {};
		virtual std::string GetName() const;

		void RemoveGameObject(std::shared_ptr<GameObject> obj);

		virtual ~Scene() = default;
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	protected:
		explicit Scene(const std::string& name);
		bool m_IsInitialized;

	private:
		std::string m_Name{};
		std::vector<std::shared_ptr<GameObject>> m_Objects{};

		static unsigned int idCounter; 
	};

}
