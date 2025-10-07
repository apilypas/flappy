#pragma once
#include <vector>
#include "Entities.hpp"
#include "BannerMessageSource.hpp"
#include "FlappyRenderer.hpp"
#include "PillarRenderer.hpp"
#include "BackgroundRenderer.hpp"
#include "LabelRenderer.hpp"
#include "PowerUpRenderer.hpp"
#include "GameBannerRenderer.hpp"
#include "SfxPlayer.hpp"

class Game {
    bool showDebug;
    int _nextPillarId = 0;

    BannerMessageSource _bannerMessageSource;

    Camera2D _camera;
    
    GameState _gameState;
    Flappy _flappy;
    std::vector<Pillar> _pillars;
    std::vector<Label> _banners;
    std::vector<PowerUp> _powerUps;
    Background _background;

    GameBanner _gameBanner;
    Label _scoreLabel;
    
    BackgroundRenderer _backgroundRenderer;
    FlappyRenderer _flappyRenderer;
    PillarRenderer _pillarRenderer;
    LabelRenderer _labelRenderer;
    PowerUpRenderer _powerUpRenderer;
    GameBannerRenderer _gameBannerRenderer;

    SfxPlayer _sfxPlayer;
public:
    void Initialize();
    void Run();
    void Uninitialize();
private:
    void Reset();
    Pillar CreatePillar(float x, float y);
    void UpdatePhysics();
    void UpdatePillars(float);
    bool HandleScore();
    void UpdateBackground(float);
    void UpdateBanners(float);
    void UpdatePowerUps(float);
    void HandleDeathState();
    bool HandlePowerUps();
    Label CreateBanner(float x, float y);
};
