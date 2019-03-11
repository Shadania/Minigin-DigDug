#pragma once
#include "BaseComponent.h"

namespace dae
{
	class ControllerComponent : public BaseComponent
	{

	};

	class InputControllerComponent final : public ControllerComponent
	{

	};

	// Write separate AI implementations by overriding this one's update function
	class AIControllerComponent : public ControllerComponent
	{

	};
}
