#include "MusicPlayer.hpp"
#include <raylib.h>

void MusicPlayer::Initialize()
{
    _music = LoadMusicStream("assets/audio/Music.mp3");
}

void MusicPlayer::Play()
{
    PlayMusicStream(_music);
    SetMusicVolume(_music, 0.5f);
}

void MusicPlayer::MakeVolumeLower()
{
    if (!_isVolumeLowered) {
        _isVolumeLowered = true;
        SetMusicVolume(_music, 0.3f);
    }
}

void MusicPlayer::MakeVolumeHigher()
{
    if (_isVolumeLowered) {
        _isVolumeLowered = false;
        SetMusicVolume(_music, 0.5f);
    }
}

void MusicPlayer::Update() { UpdateMusicStream(_music); }

void MusicPlayer::Uninitialize() { UnloadMusicStream(_music); }
