#pragma once
#include <memory>
#include <unordered_map>
#include <string>



class AudioService
{
public:
	AudioService() = default;
	virtual ~AudioService() = default;

	virtual void PlaySound(const std::string& path) = 0;
	virtual void StopSound(unsigned int id) = 0;
	virtual void PauseSound(unsigned int id) = 0;
	virtual void ResumeSound(unsigned int id) = 0;
	virtual void SetVolume(unsigned int id, float volume) = 0;
	virtual float GetVolume() const = 0;
	virtual void SetSoundLooping(unsigned int id, bool shouldLoop) = 0;

	virtual void SetMemoryBudget(size_t budgetInBytes) = 0;
	virtual void UnloadSound(unsigned int id) = 0;

private:


};

