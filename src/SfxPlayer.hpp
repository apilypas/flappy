#pragma once
#include "raylib.h"

enum SfxType {
    Jump,
    Hit,
    Point
};

class SfxPlayer {
    Sound _jumpSound;
    Sound _hitSound;
    Sound _pointSound;
    void PlayWithRandomPitch(Sound);
    void Play(Sound);
public:
    void Initialize();
    void Play(SfxType);
    void Unitialize();
};