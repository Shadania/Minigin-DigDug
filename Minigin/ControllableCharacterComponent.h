#pragma once
#include "BaseComponent.h"
#include "ControllerComponents.h"

namespace dae
{
	class ControllableCharacterComponent final : public BaseComponent
	{
	public:
		ControllableCharacterComponent();

		void SetController(ControllerComponent* pContr);

		ControllableCharacterComponent(const ControllableCharacterComponent&) = delete;
		ControllableCharacterComponent(ControllableCharacterComponent&&) noexcept = delete;
		ControllableCharacterComponent& operator=(const ControllableCharacterComponent&) = delete;
		ControllableCharacterComponent& operator=(ControllableCharacterComponent&&) noexcept = delete;

	private:
		ControllerComponent* m_pController;

	};
}
