#pragma once
#include "raylib.h"
#include "Entities.hpp"
#include "BannerMessageSource.hpp"

class Game {
    BannerMessageSource _bannerMessageSource;
    void Reset(Flappy &, Pillar *, Camera2D &, GameState &);
    void UpdatePhysics(Flappy &);
    void UpdatePillars(Pillar *, Flappy &);
    bool HandleScore(Pillar*, Flappy &, GameState &);
    void UpdateBackground(Background &);
    void UpdateBanner(Label &);
    void RandomizeBanner(Label &);
    void UpdateDeathState(Flappy &, Pillar *);
    void ShiftScreen(Flappy &, Camera2D &, Pillar *);
public:
    void Initialize();
    void Run();
    void Uninitialize();
};
