#include "MiniginPCH.h"
#include "Audio.h"
#include <iostream>
#pragma comment(lib, "sdl2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_mixer.lib")  
#include <SDL_mixer.h>
#include <string>


#pragma region RegularAudio
int dae::RegularAudio::AddSound(const std::string& file)
{
	Mix_Chunk* newSound = Mix_LoadWAV(file.c_str());

	if (newSound == nullptr)
	{
		std::string toThrow{ Mix_GetError() };
		throw std::exception(toThrow.c_str());
	}

	m_vpSounds.push_back(newSound);
	return (int)m_vpSounds.size() - 1;
}
int dae::RegularAudio::AddMusic(const std::string& file)
{
	Mix_Music* newSound = Mix_LoadMUS(file.c_str());
	m_vpMusic.push_back(newSound);
	return (int)m_vpMusic.size() - 1;
}
void dae::RegularAudio::PlaySound(unsigned int idx)
{
	Mix_PlayChannel(-1, m_vpSounds[idx], 0);
}
void dae::RegularAudio::PlayMusic(unsigned int idx)
{
	Mix_PlayMusic(m_vpMusic[idx], 0);
}
void dae::RegularAudio::StopAllSounds()
{
	Mix_HaltChannel(-1);
}
void dae::RegularAudio::StopAllMusic()
{
	Mix_HaltMusic();
}
void dae::RegularAudio::Init()
{
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
}
void dae::RegularAudio::Cleanup()
{
	for (size_t i{}; i < m_vpMusic.size(); ++i)
	{
		Mix_FreeMusic(m_vpMusic[i]);
	}
	for (size_t i{}; i < m_vpSounds.size(); ++i)
	{
		Mix_FreeChunk(m_vpSounds[i]);
	}
}
#pragma endregion RegularAudio

#pragma region LoggedAudio
int dae::LoggedAudio::AddSound(const std::string& file)
{
	std::cout << "Adding sound from " << file << ".\n";
	return 0;
}
int dae::LoggedAudio::AddMusic(const std::string& file)
{
	std::cout << "Adding music from " << file << ".\n";
	return 0;
}
void dae::LoggedAudio::PlaySound(unsigned int idx)
{
	std::cout << "Playing sound " << idx << ".\n";
}
void dae::LoggedAudio::PlayMusic(unsigned int idx)
{
	std::cout << "Playing music " << idx << ".\n";
}
void dae::LoggedAudio::StopAllSounds()
{
	std::cout << "Stopping all sounds.\n";
}
void dae::LoggedAudio::StopAllMusic()
{
	std::cout << "Stopping all music.\n";
}
void dae::LoggedAudio::Init()
{
	std::cout << "Initializing LoggedAudio...\n";
}
void dae::LoggedAudio::Cleanup()
{
	std::cout << "Cleaning up LoggedAudio...\n";
}
#pragma endregion LoggedAudio