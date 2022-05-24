#pragma once
#ifndef AGATA_AUDIO_H
#define AGATA_AUDIO_H

#include "../Dependencies/FMOD/x64/inc/fmod.hpp"
#include <vector>
#include <map>

class Audio
{
public:
	Audio() = default;

	static Audio& GetInstance()
	{
		static Audio instance;
		return instance;
	}

	Audio(Audio const&) = delete;
	void operator=(Audio const&) = delete;

	bool Init();
	bool LoadAudioFile(char* file, char* nameId, bool useChannel = false, unsigned short int id = 0);
	void PlaySoundX(FMOD::Sound*, bool bLooping = false);
	void PlaySoundOnCustomChannel(FMOD::Sound*, unsigned short int id, float channelVolume = 1.0f);
	void StopSound(unsigned short int channelId);
	void StopSoundRanged(unsigned short int from, unsigned short int to);
	void ReleaseSound(FMOD::Sound*);
	void Update();

	std::map<char*, FMOD::Sound*> GetSoundsMap() { return m_sounds; }

private:
	~Audio() = default;

	std::map<unsigned short int, FMOD::Channel*> m_channelMap;
	std::map<char*, FMOD::Sound*> m_sounds;
	FMOD::System* m_pAudioManager;
};

#endif // AGATA_AUDIO_H