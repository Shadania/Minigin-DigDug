#pragma once
#include "BaseComponent.h"

namespace dae
{
	class InputControllerComponent final : public BaseComponent
	{
	public:

		virtual void Update() override;
		virtual void Initialize() override;

	private:


	};
}