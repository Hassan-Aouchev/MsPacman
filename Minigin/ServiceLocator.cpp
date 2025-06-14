#include "ServiceLocator.h"

std::unique_ptr<AudioService> ServiceLocator::m_AudioService = nullptr;
std::unique_ptr<AudioService> ServiceLocator::m_NullAudioService = nullptr;
bool ServiceLocator::m_MuteSound = false;