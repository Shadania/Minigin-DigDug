#pragma once
#include <vector>

struct Mix_Chunk;
struct _Mix_Music;
typedef _Mix_Music Mix_Music;

namespace dae
{	
	class Audio
	{
	public:
		virtual int AddSound(const std::string&) = 0;
		virtual int AddMusic(const std::string&) = 0;
		virtual void PlaySound(unsigned int) = 0;
		virtual void PlayMusic(unsigned int) = 0;
		virtual void StopAllSounds() = 0;
		virtual void StopAllMusic() = 0;
		virtual void Init() = 0;
		virtual void Cleanup() = 0;	
	
	protected:
			std::vector<Mix_Chunk*> m_vpSounds;
			std::vector<Mix_Music*> m_vpMusic;
	};
	class NullAudio final : public Audio
	{
		virtual int AddSound(const std::string&) override { return 0; }
		virtual int AddMusic(const std::string&) override { return 0; }
		virtual void PlaySound(unsigned int) override {}
		virtual void PlayMusic(unsigned int) override {}
		virtual void StopAllSounds() override {}
		virtual void StopAllMusic() override {}
		virtual void Init() override {}
		virtual void Cleanup() override {}
	};

	class RegularAudio final : public Audio
	{
	public:
		virtual int AddSound(const std::string& file) override;
		virtual int AddMusic(const std::string& file) override;
		virtual void PlaySound(unsigned int idx) override;
		virtual void PlayMusic(unsigned int idx) override;
		virtual void StopAllSounds() override;
		virtual void StopAllMusic() override;
		virtual void Init() override;
		virtual void Cleanup() override;
	};

	class LoggedAudio final : public Audio
	{
	public:
		virtual int AddSound(const std::string& file) override;
		virtual int AddMusic(const std::string& file) override;
		virtual void PlaySound(unsigned int idx) override;
		virtual void PlayMusic(unsigned int idx) override;
		virtual void StopAllSounds() override;
		virtual void StopAllMusic() override;
		virtual void Init() override;
		virtual void Cleanup() override;
	};
}