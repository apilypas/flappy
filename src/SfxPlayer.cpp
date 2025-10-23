#include "SfxPlayer.hpp"
#include <raylib.h>

void SfxPlayer::PlayWithRandomPitch(Sound sound)
{
    float pitch = (float)GetRandomValue(80, 120) / 100.0f;
    SetSoundPitch(sound, pitch);
    PlaySound(sound);
}

void SfxPlayer::Play(Sound sound)
{
    SetSoundPitch(sound, 1);
    PlaySound(sound);
}

void SfxPlayer::Initialize()
{
    _jumpSound = LoadSound("assets/audio/Jump.wav");
    _hitSound = LoadSound("assets/audio/Hit.wav");
    _pointSound = LoadSound("assets/audio/Point.wav");
    _powerUpSound = LoadSound("assets/audio/PowerUp.wav");
    _unlockSound = LoadSound("assets/audio/Unlock.wav");
}

void SfxPlayer::Play(SfxType sfxType)
{
    if (sfxType == SfxType::Jump)
        PlayWithRandomPitch(_jumpSound);
    else if (sfxType == SfxType::Point)
        PlayWithRandomPitch(_pointSound);
    else if (sfxType == SfxType::Hit)
        Play(_hitSound);
    else if (sfxType == SfxType::PowerUp)
        PlayWithRandomPitch(_powerUpSound);
    else if (sfxType == SfxType::Unlock)
        PlayWithRandomPitch(_unlockSound);
}

void SfxPlayer::Uninitialize()
{
    UnloadSound(_jumpSound);
    UnloadSound(_hitSound);
    UnloadSound(_pointSound);
    UnloadSound(_powerUpSound);
    UnloadSound(_unlockSound);
}
