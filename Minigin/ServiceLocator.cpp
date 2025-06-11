#include "ServiceLocator.h"

std::unique_ptr<AudioService> ServiceLocator::m_AudioService = nullptr;