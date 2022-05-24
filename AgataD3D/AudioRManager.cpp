#include "pch.h"
#include "Audio.h"
#include "AudioRManager.h"
#include <iostream>

void AudioRManager::LoadAudioFiles() {
	Audio::GetInstance().LoadAudioFile((char*)"Assets/Audio/Juego.mp3", (char*)"Juego", true, 1);
	Audio::GetInstance().LoadAudioFile((char*)"Assets/Audio/Recoger.mp3", (char*)"Recoger", true, 2);
	Audio::GetInstance().LoadAudioFile((char*)"Assets/Audio/Recoger2.mp3", (char*)"Recoger2", true, 3);
	Audio::GetInstance().LoadAudioFile((char*)"Assets/Audio/tractor.mp3", (char*)"Tractor", true, 4);
	Audio::GetInstance().LoadAudioFile((char*)"Assets/Audio/TemaPrincipalR.mp3", (char*)"JuegoR", true, 5);
	Audio::GetInstance().LoadAudioFile((char*)"Assets/Audio/win.mp3", (char*)"win", true, 6);
	Audio::GetInstance().LoadAudioFile((char*)"Assets/Audio/perderS.mp3", (char*)"lose", true, 7);


}

void AudioRManager::ReleaseAudioFiles() {
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find((char*)"Juego")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find((char*)"Recoger")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find((char*)"Recoger2")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find((char*)"Tractor")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find((char*)"JuegoR")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find((char*)"win")->second);
	Audio::GetInstance().ReleaseSound(Audio::GetInstance().GetSoundsMap().find((char*)"lose")->second);
}

