#pragma once
#ifndef AGATA_AUDIOR_MANAGER_H
#define AGATA_AUDIOR_MANAGER_H

#include <vector>
#include <string>
#include <map>

class AudioRManager
{
public:
	~AudioRManager() = default;

	static AudioRManager& GetInstance()
	{
		static AudioRManager instance;
		return instance;
	}

	AudioRManager(AudioRManager const&) = delete;
	void operator=(AudioRManager const&) = delete;

	void LoadAudioFiles();
	void ReleaseAudioFiles();

private:
	AudioRManager() = default;

	std::map<char*, unsigned char*> m_resourceId;
};

#endif // AGATA_AUDIOR_MANAGER_H