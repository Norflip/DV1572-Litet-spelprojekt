#pragma once
#include <Audio.h>
#include <unordered_map>

class SoundHandler
{
	public:
		SoundHandler();
		~SoundHandler();
		void LoadSound(std::string key, std::string filePath);
		void PlaySound(std::string key, float volume);
		bool HasKey(std::string key) const;
		void DeleteTrack(std::string key);
		void SetGlobalVolume(float volume);
		float GetGlobalVolume();
		void StopSound();
		void SetLevelSoundtrack(std::string soundname) { this->levelsoundname = soundname; }
		std::string GetLevelSoundtrack() { return this->levelsoundname; }

	private:
		DirectX::AudioEngine* audioEngine;
		DirectX::SoundEffect* soundEffect;

		std::unordered_map<std::string, DirectX::SoundEffect*> soundMap;
		std::string levelsoundname;
};