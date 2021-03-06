#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;

	class Scene
	{
	public:
		void AddToScene(const std::shared_ptr<GameObject>& object);

		virtual void FixedUpdate();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render() const;

		virtual void Initialize() = 0;
		virtual std::string GetName() const;

		void RemoveGameObject(size_t objID);

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
		std::vector<std::shared_ptr<GameObject>> m_vspObjects{};
		std::vector<size_t> m_vObjIdsToDelete{};

		static unsigned int m_IdCounter; 
	};

}
