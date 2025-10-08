#pragma once

#include <raylib.h>

class MusicPlayer {
    Music _music;
    bool _isVolumeLowered = false;
public:
    void Initialize();
    void Play();
    void MakeVolumeLower();
    void MakeVolumeHigher();
    void Update();
    void Uninitialize();
};