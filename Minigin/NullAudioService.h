#pragma once
#include "AudioService.h"
#include <string>

class NullAudioService:public AudioService
{
public:
	NullAudioService() = default;
	~NullAudioService() = default;
	void PlaySound(const std::string& )override{}
	void StopSound(unsigned int ) override{}
	void PauseSound(unsigned int ) override{}
	void ResumeSound(unsigned int ) override{}
	void SetVolume(unsigned int , float ) override{}
	float GetVolume() const override { return 0.f; }
	void SetSoundLooping(unsigned int , bool ) override{}
	void SetMemoryBudget(size_t ) override{}
	void UnloadSound(unsigned int ) override {}

};
