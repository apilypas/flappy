#pragma once
#include <vector>
#include "Entities.hpp"
#include "BannerMessageSource.hpp"

class Game {
    BannerMessageSource _bannerMessageSource;
    void Reset(Flappy&, std::vector<Label>&, std::vector<Pillar>&, std::vector<PowerUp>&, GameState&);
    Pillar CreatePillar(float x, float y);
    void UpdatePhysics(Flappy&);
    void UpdatePillars(std::vector<Pillar>&, Flappy&, float);
    bool HandleScore(std::vector<Pillar>&, Flappy&, GameState&);
    void UpdateBackground(Background&, float);
    void UpdateBanners(std::vector<Label>&, float);
    void UpdatePowerUps(std::vector<PowerUp>&, std::vector<Pillar>&, float);
    void HandleDeathState(Flappy&, std::vector<Pillar>&);
    bool HandlePowerUps(Flappy&, std::vector<PowerUp>&);
    Label CreateBanner(float x, float y);
public:
    void Initialize();
    void Run();
    void Uninitialize();
};
