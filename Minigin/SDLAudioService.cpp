#include "SDLAudioService.h"
#include <SDL_mixer.h>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <algorithm>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <SDL.h>
#include "ResourceManager.h"

namespace {
    enum class AudioCommandType {
        Play
    };

    struct AudioCommand {
        AudioCommandType type;
        std::string filePath;
    };
}

class SDLAudioService::SDLAudioServiceImpl {
public:
    struct CacheEntry {
        Mix_Chunk* chunk;
        size_t size;
    };
    std::unordered_map<std::string, std::list<std::string>::iterator> cacheMap;
    std::unordered_map<std::string, CacheEntry> soundCache;
    std::list<std::string> lruList;
    std::mutex queueMutex;
    size_t currentMemoryUsage = 0;
    size_t memoryBudget = 50 * 1024 * 1024;
    std::mutex cacheMutex;

    std::condition_variable queueCondition;
    std::queue<AudioCommand> commandQueue;
    std::jthread m_Thread;

    SDLAudioServiceImpl() {
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            return;
        }

        if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
            std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        }
        m_Thread = std::jthread([this](std::stop_token stopToken) {
            Run(stopToken);
            });
    }

    ~SDLAudioServiceImpl() {
        m_Thread.request_stop();
        queueCondition.notify_one();

        for (auto& [path, cache] : soundCache) {
            Mix_FreeChunk(cache.chunk);
        }
        soundCache.clear();

        Mix_CloseAudio();
        SDL_QuitSubSystem(SDL_INIT_AUDIO);
    }

    void SetMemoryBudget(size_t budgetInBytes) {
        std::lock_guard lock(cacheMutex);
        memoryBudget = budgetInBytes;
        EvictIfNeeded();
    }

    void QueuePlaySound(const std::string& filePath) {
        std::unique_lock lock(queueMutex);
        commandQueue.push(AudioCommand{ AudioCommandType::Play, filePath });
        queueCondition.notify_one();
    }

    void Run(std::stop_token stopToken) {
        while (!stopToken.stop_requested()) {
            std::unique_lock lock(queueMutex);

            queueCondition.wait(lock, [&] {
                return !commandQueue.empty() || stopToken.stop_requested();
                });

            if (commandQueue.empty() && stopToken.stop_requested()) {
                break;
            }

            while (!commandQueue.empty()) {
                auto command = commandQueue.front();
                commandQueue.pop();
                lock.unlock();

                if (command.type == AudioCommandType::Play) {
                    Mix_Chunk* chunk = LoadOrGetChunk(command.filePath);
                    if (chunk) {
                        if (Mix_PlayChannel(-1, chunk, 0) == -1) {
                            std::cerr << "Mix_PlayChannel error: " << Mix_GetError() << std::endl;
                        }
                    }
                }

                lock.lock();
            }
        }
    }

private:
    Mix_Chunk* LoadOrGetChunk(const std::string& path) {
        std::lock_guard lock(cacheMutex);

        // Check if the sound is already in the cache
        auto it = soundCache.find(path);
        if (it != soundCache.end()) {
            // Move the accessed sound to the front of the LRU list
            lruList.erase(cacheMap[path]);
            lruList.push_front(path);
            cacheMap[path] = lruList.begin();
            return it->second.chunk;
        }

        // Load the sound
        Mix_Chunk* chunk = Mix_LoadWAV(path.c_str());
        if (!chunk) {
            std::cerr << "Failed to load sound: " << path << " | " << Mix_GetError() << std::endl;
            return nullptr;
        }

        size_t chunkSize = chunk->alen;

        // Add to cache
        soundCache[path] = { chunk, chunkSize };
        lruList.push_front(path);
        cacheMap[path] = lruList.begin();
        currentMemoryUsage += chunkSize;

        // Evict if memory budget is exceeded
        EvictIfNeeded();

        return chunk;
    };

    void EvictIfNeeded() {
        while (currentMemoryUsage > memoryBudget && !lruList.empty()) {
            // Remove the least recently used sound
            const std::string& lruPath = lruList.back();
            auto it = soundCache.find(lruPath);

            if (it != soundCache.end()) {
                Mix_FreeChunk(it->second.chunk);
                currentMemoryUsage -= it->second.size;
                soundCache.erase(it);
            }

            cacheMap.erase(lruPath);
            lruList.pop_back();
        }
    }
};

SDLAudioService::SDLAudioService()
    : m_pImpl(std::make_unique<SDLAudioServiceImpl>()) {

}

SDLAudioService::~SDLAudioService() = default;

void SDLAudioService::PlaySound(const std::string& path) {
    m_pImpl->QueuePlaySound(ResourceManager::GetInstance().GetDataPath().string() + path);
}

void SDLAudioService::StopSound(unsigned int /*id*/) {
    Mix_HaltChannel(-1); 
}

void SDLAudioService::PauseSound(unsigned int /*id*/) {
    Mix_Pause(-1);
}

void SDLAudioService::ResumeSound(unsigned int /*id*/) {
    Mix_Resume(-1);
}

void SDLAudioService::SetVolume(unsigned int /*id*/, float /*volume*/) {
    
}

float SDLAudioService::GetVolume() const {
    return 1.0f;
}

void SDLAudioService::SetSoundLooping(unsigned int /*id*/, bool /*shouldLoop*/) {
    
}

void SDLAudioService::SetMemoryBudget(size_t /*budgetInBytes*/) {

}

void SDLAudioService::UnloadSound(unsigned int /*id*/) {
    
}
