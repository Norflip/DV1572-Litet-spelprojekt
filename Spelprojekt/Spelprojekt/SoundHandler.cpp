#include "SoundHandler.h"
#include "Logger.h"



SoundHandler::SoundHandler()
{
	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;
#ifdef _DEBUG
	eflags = eflags | DirectX::AudioEngine_Debug;
#endif
	this->audioEngine = new DirectX::AudioEngine(eflags);
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	assert(SUCCEEDED(hr));
}

void SoundHandler::LoadSound(std::string key, std::string filePath)
{
	std::wstring convertString(filePath.begin(), filePath.end());
	if (!HasKey(key))
	{
		soundEffect = new DirectX::SoundEffect(audioEngine, convertString.c_str());
		soundMap.insert({ key, soundEffect });
	}
}

void SoundHandler::PlaySound(std::string key)
{
	if (HasKey(key))
	{
		soundMap[key]->Play();
	}	
}

bool SoundHandler::HasKey(std::string key) const
{
	return soundMap.find(key) != soundMap.end();
}
