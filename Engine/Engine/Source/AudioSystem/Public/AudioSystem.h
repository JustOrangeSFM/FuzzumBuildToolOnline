#pragma once
#include <cstdint>

namespace Audio
{
    using AudioFlags = uint8_t;

    AudioFlags MakeFlags(int channel, bool applyMaster = true);

    void SetMasterVolume(float volume);
    void SetChannelVolume(int channel, float volume);

    int Play(const char* filepath, AudioFlags flags, bool loop = false, uint64_t startTimeMs = 0);
    void Stop(int soundId);
    void StopAll();

    // Добавь эти две функции для модуля:
    void InternalInitialize();   // или просто использовать Play() — он сам инициализирует
    void InternalShutdown();
}