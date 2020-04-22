#pragma once
#include <Audio.h>
#include <unordered_map>

class SoundHandler
{
	public:
		SoundHandler();
		void LoadSound(std::string key, std::string filePath);
		void PlaySound(std::string key);
		bool HasKey(std::string key) const;
	private:
		DirectX::AudioEngine* audioEngine;
		DirectX::SoundEffect* soundEffect;	

		std::unordered_map<std::string, DirectX::SoundEffect*> soundMap;
};