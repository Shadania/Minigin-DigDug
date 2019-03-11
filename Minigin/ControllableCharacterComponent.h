#pragma once
#include "BaseComponent.h"
#include "ControllerComponents.h"

namespace dae
{
	class ControllableCharacterComponent final : public BaseComponent
	{
	public:

		void SetController(ControllerComponent* pContr);

	private:
		ControllerComponent* m_pController;

	};
}
