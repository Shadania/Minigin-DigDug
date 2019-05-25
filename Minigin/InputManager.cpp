#include "MiniginPCH.h"
#include "InputManager.h"
#include <SDL.h>

bool dae::InputManager::Update()
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
		}
	}
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