#pragma once
#include "ModuleManager/IModule.h"

class IAudioModule : public IModule
{
public:
    using AudioFlags = uint8_t;

    
    virtual void SetMasterVolume(float volume) = 0;
    virtual void SetChannelVolume(int channel, float volume) = 0;

    
    virtual int Play(const char* filepath, AudioFlags flags, bool loop = false, uint64_t startTimeMs = 0) = 0;
    virtual void PlaySound(const char* path, bool loop = false) { 
        Play(path, MakeFlags(0, true), loop); 
    }

    
    virtual void Stop(int soundId) = 0;
    virtual void StopAll() = 0;

    
    virtual AudioFlags MakeFlags(int channel, bool applyMaster = true) = 0;
};