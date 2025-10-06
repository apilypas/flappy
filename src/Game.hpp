#pragma once
#include "raylib.h"
#include "Entities.hpp"
#include "BannerMessageSource.hpp"

class Game {
    BannerMessageSource _bannerMessageSource;
    void Reset(Flappy&, std::vector<Pillar>&, Camera2D&, GameState&);
    Pillar CreatePillar(float x, float y);
    void UpdatePhysics(Flappy&);
    void UpdatePillars(std::vector<Pillar>&, Flappy&);
    bool HandleScore(std::vector<Pillar>&, Flappy&, GameState&);
    void UpdateBackground(Background&);
    void UpdateBanner(Label&);
    void RandomizeBanner(Label&);
    void UpdateDeathState(Flappy&, std::vector<Pillar>&);
    void ShiftScreen(Flappy&, Camera2D&, std::vector<Pillar>&, Background&, Label&);
public:
    void Initialize();
    void Run();
    void Uninitialize();
};
