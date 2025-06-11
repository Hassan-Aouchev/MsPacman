#pragma once
#include <memory>
#include "AudioService.h"

class ServiceLocator
{
public:

	static void ProvideAudioService(std::unique_ptr<AudioService> service)
	{
		m_AudioService = std::move(service);
	}

	static AudioService* GetAudioService()
	{
		return m_AudioService.get();
	}

private:

	static std::unique_ptr<AudioService> m_AudioService;
};