#include "pch.h"
#include "Audio.h"

bool Audio::Init()
{
	FMOD_RESULT result;

	result = FMOD::System_Create(&m_pAudioManager);

	if (result != FMOD_OK)
	{
		printf("FMOD ERROR: No se puede inicializar la biblioteca FMOD");
		return false;
	}

	result = m_pAudioManager->init(50, FMOD_INIT_NORMAL, NULL);

	if (result != FMOD_OK)
	{
		printf("FMOD ERROR: No se puede inicializar la biblioteca FMOD");
		return false;
	}

	return true;
}

bool Audio::LoadAudioFile(char* pFile, char* pNnameId, bool useChannel, unsigned short int id)
{
	FMOD_RESULT result;

	FMOD::Sound* pSound;
	result = m_pAudioManager->createSound(pFile, FMOD_DEFAULT, NULL, &pSound);

	if (result != FMOD_OK)
	{
		printf("FMOD ERROR: No se puede crear el sonido, asegúrese de que la ruta del archivo sea correcta.");
		return false;
	}

	m_sounds[pNnameId] = pSound;

	if (useChannel)
	{
		FMOD::Channel* newChannel = nullptr;
		m_channelMap[id] = newChannel;
	}

	return true;
}

void Audio::PlaySoundX(FMOD::Sound* pSound, bool bLooping)
{
	if (!bLooping)
		pSound->setMode(FMOD_LOOP_OFF);
	else
	{
		pSound->setMode(FMOD_LOOP_NORMAL);
		pSound->setLoopCount(-1);
	}

	m_pAudioManager->playSound(pSound, nullptr, false, 0);
}

void Audio::PlaySoundOnCustomChannel(FMOD::Sound* pSound, unsigned short int id, float channelVolume)
{
	// Comprueba si ya se está reproduciendo un sonido
	bool isSoundPlaying;
	m_channelMap[id]->isPlaying(&isSoundPlaying);
	m_channelMap[id]->setVolume(channelVolume);

	// Si no se está reproduciendo ningún sonido, continua y reproduce un sonido
	if (!isSoundPlaying)
		m_pAudioManager->playSound(pSound, nullptr, false, &m_channelMap[id]);
}

void Audio::StopSound(unsigned short int channelId)
{
	m_channelMap[channelId]->stop();
}

void Audio::StopSoundRanged(unsigned short int from, unsigned short int to)
{
	for (auto i = from; i <= to; ++i)
	{
		m_channelMap[i]->stop();
	}
}

void Audio::ReleaseSound(FMOD::Sound* pSound)
{
	pSound->release();
}

void Audio::Update()
{
	m_pAudioManager->update();
}