#pragma once
#include <raylib.h>

enum class SfxType {
    Jump,
    Hit,
    Point,
    PowerUp
};

class SfxPlayer {
    Sound _jumpSound;
    Sound _hitSound;
    Sound _pointSound;
    Sound _powerUpSound;
    void PlayWithRandomPitch(Sound);
    void Play(Sound);
public:
    void Initialize();
    void Play(SfxType);
    void Uninitialize();
};