#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>

/*

bool dae::InputManager::ProcessInput()
{
	ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &currentState);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
	}

	return true;
}

bool dae::InputManager::IsPressed(ControllerButton button) const
{
	switch (button)
	{
	case ControllerButton::ButtonA:
		return currentState.Gamepad.wButtons & XINPUT_GAMEPAD_A;
	case ControllerButton::ButtonB:
		return currentState.Gamepad.wButtons & XINPUT_GAMEPAD_B;
	case ControllerButton::ButtonX:
		return currentState.Gamepad.wButtons & XINPUT_GAMEPAD_X;
	case ControllerButton::ButtonY:
		return currentState.Gamepad.wButtons & XINPUT_GAMEPAD_Y;
	default: return false;
	}
}

*/

bool dae::InputManager::Update()
{
	for (auto cmd : m_Commands)
	{
		cmd.second.keyDown = false;
		cmd.second.keyUp = false;
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		auto key = e.key.keysym.sym;

		switch (e.type)
		{
		case SDL_QUIT:
			return false;
		case SDL_KEYDOWN:
			for (std::pair<const std::string, Command>& cmd : m_Commands)
			{
				if (cmd.second.cmdKeycode == key)
				{
					if (cmd.second.key)
						break;

					cmd.second.key = true;
					cmd.second.keyDown = true;
					break;
				}
			}
			break;
		case SDL_KEYUP:
			for (std::pair<const std::string, Command>& cmd : m_Commands)
			{
				if (cmd.second.cmdKeycode == key)
				{
					cmd.second.key = false;
					cmd.second.keyUp = true;
					break;
				}
			}
			break;
		}
	}
	return true;
}
bool dae::InputManager::Key(std::string action)
{
	return m_Commands[action].key;
}
bool dae::InputManager::KeyDown(std::string action)
{
	return m_Commands[action].keyDown;
}
bool dae::InputManager::KeyUp(std::string action)
{
	return m_Commands[action].keyUp;
}
void dae::InputManager::AddCommand(const std::string& cmdName, SDL_Keycode cmdKey)
{
	m_Commands[cmdName] = Command(cmdKey);
}