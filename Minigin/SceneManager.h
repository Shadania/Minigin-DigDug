#pragma once

namespace dae
{
	class Scene;
	class SceneManager
	{
	public:
		Scene & CreateScene(const std::string& name);
		void AddScene(std::shared_ptr<Scene> scene);

		void FixedUpdate();
		void Update();
		void LateUpdate();
		void Render();

		void SetActiveScene(const std::string& name);


	private:
		std::vector<std::shared_ptr<Scene>> m_Scenes;
		std::shared_ptr<Scene> m_spActiveScene;
	};

}
