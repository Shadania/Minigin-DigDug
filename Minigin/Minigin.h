#pragma once

struct SDL_Window;

namespace dae
{
	struct WindowInfo {
		float width = 0.0f;
		float height = 0.0f;
	};


	class Minigin
	{
		const int msPerFrame = 16; //16 for 60 fps, 33 for 30 fps
		SDL_Window* window{};
	public:
		void Initialize(int wWidth, int wHeight);
		void LoadGame() const;
		void Cleanup();
		void Run();

	private:

	};
}