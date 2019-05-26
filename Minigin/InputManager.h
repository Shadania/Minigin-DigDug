#pragma once
#include <XInput.h>
#pragma comment(lib, "XInput.lib")
#include <unordered_map>

typedef int32_t SDL_Keycode;

namespace dae
{
	// Keyboard
	struct Command
	{
		Command()
			:cmdKeycode{ '\0' }
		{}
		Command(SDL_Keycode key)
			:cmdKeycode{key}
		{}

		SDL_Keycode cmdKeycode;
		bool key{ false };
		bool keyUp{ false };
		bool keyDown{ false };
	};
	struct Axis
	{
		Axis()
			:positiveButton{'\0'}
			,negativeButton{'\0'}
			,val{0.0f}
		{}
		Axis(SDL_Keycode posBtn, SDL_Keycode negBtn)
			:positiveButton{posBtn}
			,negativeButton{negBtn}
			,val{0.0f}
		{}

		SDL_Keycode positiveButton;
		SDL_Keycode negativeButton;
		float val;
	};

	// Controller
	enum class ControllerButton
	{
		ButtonA,
		DPLeft,
		DPRight,
		DPUp,
		DPDown
	};




	class InputManager
	{
	public:

		bool Update();

		void AddCommand(const std::string& cmdName, SDL_Keycode cmdKey);
		void AddAxis(const std::string& axisName, SDL_Keycode posButton, SDL_Keycode negButton);

		bool Key(std::string&& action);
		bool KeyDown(std::string&& action);
		bool KeyUp(std::string&& action);

		float GetAxis(std::string&& axis);

		bool GetControllerKey(size_t playerIdx, ControllerButton btn);

	private:

		std::unordered_map<std::string, Command> m_Commands;
		std::unordered_map<std::string, Axis> m_Axes;

		XINPUT_STATE m_Controllers[2]{};


		bool HandleKeyboardInput();
		bool HandleControllerInput();

		bool m_Controller1Valid = true;
		bool m_Controller2Valid = true;
	};

}
