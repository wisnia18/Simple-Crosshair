#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"
#include "Audio.h"

static ma_engine g_engine;
static bool g_initialized = false;

bool Audio::s_enableSound = true;
float Audio::s_volume = 0.5f;

char Audio::s_soundPath[256] = "assets/click.wav";

bool Audio::Initialize() {
    if (ma_engine_init(NULL, &g_engine) != MA_SUCCESS) {
        return false;
    }
    ma_engine_set_volume(&g_engine, s_volume);
    g_initialized = true;
    return true;
}

void Audio::Shutdown() {
    if (g_initialized) {
        ma_engine_uninit(&g_engine);
        g_initialized = false;
    }
}

void Audio::Play(const std::string& filepath) {
    if (!g_initialized || filepath.empty()) return;
    ma_engine_play_sound(&g_engine, filepath.c_str(), NULL);
}

void Audio::SetVolume(float volume) {
    s_volume = volume;
    if (g_initialized) {
        ma_engine_set_volume(&g_engine, volume);
    }
}

void Audio::PlaySoundEffect() {
    if (s_enableSound) {
        Play(std::string(s_soundPath));
    }
}
