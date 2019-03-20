#pragma once

namespace dae
{
	class GameObject;

	class BaseComponent
	{
	public:
		BaseComponent(std::shared_ptr<GameObject> pObj);
		virtual ~BaseComponent() = default;
		virtual void Update() = 0;

		std::shared_ptr<dae::GameObject> GameObj() const;
		void SetGameObj(std::shared_ptr<GameObject> newObj);
		std::string GetType() const;

		BaseComponent(const BaseComponent&) = delete;
		BaseComponent(BaseComponent&&) noexcept = delete;
		BaseComponent& operator=(const BaseComponent&) = delete;
		BaseComponent& operator=(BaseComponent&&) noexcept = delete;

	protected:
		std::weak_ptr<GameObject> m_spMyObj;
		std::string m_Type;
	};
}