#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>



dae::InputManager::InputManager()
{
	m_spMouseDown = std::make_shared<Event>();
	m_spMouseUp = std::make_shared<Event>();
	m_spMouseMoved = std::make_shared<Event>();
}

bool dae::InputManager::Update()
{
	if (m_StopGame)
		return false;

	if (!HandleKeyboardInput())
		return false;
	if (!HandleControllerInput())
		return false;

	return true;
}
bool dae::InputManager::Key(std::string&& action)
{
	return m_Commands[std::move(action)].key;
}
bool dae::InputManager::KeyDown(std::string&& action)
{
	return m_Commands[std::move(action)].keyDown;
}
bool dae::InputManager::KeyUp(std::string&& action)
{
	return m_Commands[std::move(action)].keyUp;
}

void dae::InputManager::AddCommand(const std::string& cmdName, SDL_Keycode cmdKey)
{
	m_Commands[cmdName] = Command(cmdKey);
}
void dae::InputManager::AddAxis(const std::string& axisName, SDL_Keycode posButton, SDL_Keycode negButton)
{
	m_Axes[axisName] = Axis(posButton, negButton);
}
float dae::InputManager::GetAxis(std::string&& axis)
{
	return m_Axes[std::move(axis)].val;
}

bool dae::InputManager::HandleKeyboardInput()
{
	for (auto& cmd : m_Commands)
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
			// command buttons
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
			// axes
			for (std::pair<const std::string, Axis>& axis : m_Axes)
			{
				if (axis.second.positiveButton == key)
					axis.second.val = 1.0f;
				else if (axis.second.negativeButton == key)
					axis.second.val = -1.0f;
			}
			break;
		case SDL_KEYUP:
			if (key == SDLK_ESCAPE)
				return false;
			
			for (std::pair<const std::string, Command>& cmd : m_Commands)
			{
				if (cmd.second.cmdKeycode == key)
				{
					cmd.second.key = false;
					cmd.second.keyUp = true;
					break;
				}
			}
			for (std::pair<const std::string, Axis>& axis : m_Axes)
			{
				if (axis.second.positiveButton == key || axis.second.negativeButton == key)
					axis.second.val = 0;
			}
			break;
		case SDL_MOUSEBUTTONDOWN:
			UpdateMousePos();
			m_spMouseDown->Invoke();
			break;
		case SDL_MOUSEMOTION:
			UpdateMousePos();
			m_spMouseMoved->Invoke();
			break;
		case SDL_MOUSEBUTTONUP:
			UpdateMousePos();
			m_spMouseUp->Invoke();
			break;
		}
	}
	return true;
}
bool dae::InputManager::HandleControllerInput()
{
	m_Controller1Valid = XInputGetState(0, &m_Controllers[0]) == ERROR_SUCCESS;
	m_Controller2Valid = XInputGetState(1, &m_Controllers[1]) == ERROR_SUCCESS;
	return true;
}

bool dae::InputManager::GetControllerKey(size_t playerIdx, ControllerButton btn)
{
	if (playerIdx == 0)
		if (!m_Controller1Valid)
			return false;
	if (playerIdx == 1)
		if (!m_Controller2Valid)
			return false;

	switch (btn)
	{
	case ControllerButton::ButtonA:
		return m_Controllers[playerIdx].Gamepad.wButtons & XINPUT_GAMEPAD_A;
		break;

	case ControllerButton::DPDown:
		return m_Controllers[playerIdx].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN;
		break;

	case ControllerButton::DPLeft:
		return m_Controllers[playerIdx].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT;
		break;

	case ControllerButton::DPRight:
		return m_Controllers[playerIdx].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT;
		break;

	case ControllerButton::DPUp:
		return m_Controllers[playerIdx].Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP;
		break;
	}

	return false;
}
void dae::InputManager::UpdateMousePos()
{
	int x{}, y{};
	SDL_GetMouseState(&x, &y);
	m_MousePos.x = float(x);
	m_MousePos.y = float(y);
}