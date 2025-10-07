#include <raylib.h>
#include "SfxPlayer.hpp"

void SfxPlayer::PlayWithRandomPitch(Sound sound)
{
    auto pitch = (float)GetRandomValue(80, 120) / 100.0f;
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
}

void SfxPlayer::Play(SfxType sfxType)
{
    if (sfxType == SfxType::SfxJump)
        PlayWithRandomPitch(_jumpSound);
    else if (sfxType == SfxType::SfxPoint)
        PlayWithRandomPitch(_pointSound);
    else if (sfxType == SfxType::SfxHit)
        Play(_hitSound);
    else if (sfxType == SfxType::SfxPowerUp)
        PlayWithRandomPitch(_powerUpSound);
}

void SfxPlayer::Uninitialize()
{
    UnloadSound(_jumpSound);
    UnloadSound(_hitSound);
    UnloadSound(_pointSound);
    UnloadSound(_powerUpSound);
}
