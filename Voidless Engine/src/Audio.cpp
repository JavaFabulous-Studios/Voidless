#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "Voidless/Audio.hpp"
#include <iostream>

namespace Voidless {

    static ma_engine s_AudioEngine;
    static ma_sound s_Music;
    static bool s_MusicPlaying = false;
    static bool s_EngineInitialized = false;

    void Audio::Init() {
        ma_result result = ma_engine_init(nullptr, &s_AudioEngine);
        if (result != MA_SUCCESS) {
            std::cerr << "Failed to initialize audio engine!" << std::endl;
            return;
        }
        s_EngineInitialized = true;
        std::cout << "Audio engine initialized." << std::endl;
    }

    void Audio::Shutdown() {
        if (!s_EngineInitialized) return;

        if (s_MusicPlaying) {
            ma_sound_uninit(&s_Music);
        }
        ma_engine_uninit(&s_AudioEngine);
        s_EngineInitialized = false;
    }

    void Audio::PlayMusic(const std::string& filePath, bool loop) {
        if (!s_EngineInitialized) return;

        if (s_MusicPlaying) {
            ma_sound_uninit(&s_Music);
            s_MusicPlaying = false;
        }

        ma_result result = ma_sound_init_from_file(&s_AudioEngine, filePath.c_str(), 0, nullptr, nullptr, &s_Music);
        if (result != MA_SUCCESS) {
            std::cerr << "Failed to load music file: " << filePath << " (Error: " << result << ")" << std::endl;
            return;
        }

        ma_sound_set_looping(&s_Music, loop ? MA_TRUE : MA_FALSE);
        ma_sound_start(&s_Music);
        s_MusicPlaying = true;
        std::cout << "Playing music: " << filePath << std::endl;
    }

    void Audio::StopMusic() {
        if (s_MusicPlaying) {
            ma_sound_stop(&s_Music);
        }
    }

    void Audio::SetVolume(float volume) {
        if (s_EngineInitialized) {
            ma_engine_set_volume(&s_AudioEngine, volume);
        }
    }

}
