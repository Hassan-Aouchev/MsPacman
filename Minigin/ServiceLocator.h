#pragma once
#include <memory>
#include "AudioService.h"
#include "NullAudioService.h"

#include "Command.h"

class ServiceLocator
{
public:

	static void ProvideAudioService(std::unique_ptr<AudioService> service)
	{
		if (service)
		{
			m_AudioService = std::move(service);
		}
	}

	static AudioService& GetAudioService()
	{
		if(m_AudioService&&!m_MuteSound)
		return *m_AudioService.get();
		else
		{
			if (!m_NullAudioService){
				m_NullAudioService = std::make_unique<NullAudioService>();
			}
			return *m_NullAudioService.get();
		}
	}
	static void ToggleSound() { m_MuteSound = !m_MuteSound; }

private:

	static std::unique_ptr<AudioService> m_AudioService;
	static std::unique_ptr<AudioService> m_NullAudioService;
	static bool m_MuteSound;
};

class MuteCommand :public Command
{
public:
	virtual void Execute() { ServiceLocator::ToggleSound(); }
};