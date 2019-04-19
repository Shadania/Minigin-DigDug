#pragma once
#include <XInput.h>
#include <unordered_map>

typedef int32_t SDL_Keycode;

namespace dae
{
	struct Command
	{
		Command()
		{}
		Command(SDL_Keycode key)
			:cmdKeycode{key}
		{}

		SDL_Keycode cmdKeycode{ '\0' };
		bool key{ false };
		bool keyUp{ false };
		bool keyDown{ false };
	};

	class InputManager
	{
	public:
		// bool ProcessInput();
		// bool IsPressed(ControllerButton button) const;

		bool Update();

		void AddCommand(const std::string& cmdName, SDL_Keycode cmdKey);

		bool Key(std::string action);
		bool KeyDown(std::string action);
		bool KeyUp(std::string action);


	private:
		// XINPUT_STATE currentState{};

		std::unordered_map<std::string, Command> m_Commands;
	};

}
