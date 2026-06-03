#pragma once
#include <string>

class Audio {
public:
    static bool Initialize();
    static void Shutdown();

    static void Play(const std::string& filepath);

    static void PlaySoundEffect();

    static void SetVolume(float volume);

    static bool s_enableSound;
    static float s_volume;

    static char s_soundPath[256];
};
