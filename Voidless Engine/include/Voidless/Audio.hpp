#pragma once

#include <string>

namespace Voidless {

    class Audio {
    public:
        static void Init();
        static void Shutdown();

        static void PlayMusic(const std::string& filePath, bool loop = true);
        static void StopMusic();
        static void SetVolume(float volume);

    private:
        // Internal state handled in .cpp
    };

}
