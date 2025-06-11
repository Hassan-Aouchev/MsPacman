#pragma once
#include <memory>
#include <string>
#include "AudioService.h"

class SDLAudioService : public AudioService {
public:
    SDLAudioService();
    ~SDLAudioService();

    // Core audio functions
    virtual void PlaySound(const std::string& path) override;
    virtual void StopSound(unsigned int id) override;
    virtual void PauseSound(unsigned int id) override;
    virtual void ResumeSound(unsigned int id) override;
    virtual void SetVolume(unsigned int id, float volume) override;
    virtual float GetVolume() const override;
    virtual void SetSoundLooping(unsigned int id, bool shouldLoop) override;

    // New memory management functions
    virtual void SetMemoryBudget(size_t budgetInBytes) override;
    virtual void UnloadSound(unsigned int id) override;

private:
    class SDLAudioServiceImpl;
    std::unique_ptr<SDLAudioServiceImpl> m_pImpl;
};