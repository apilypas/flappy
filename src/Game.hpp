#pragma once
#include <vector>
#include "Entities.hpp"
#include "BannerMessageSource.hpp"
#include "FlappyRenderer.hpp"
#include "PillarRenderer.hpp"
#include "BackgroundRenderer.hpp"
#include "LabelRenderer.hpp"
#include "PowerUpRenderer.hpp"
#include "SfxPlayer.hpp"

class Game {
    bool showDebug;
    BannerMessageSource _bannerMessageSource;
    int _nextPillarId = 0;
    GameState _gameState;
    Flappy _flappy;
    std::vector<Pillar> _pillars;
    std::vector<Label> _banners;
    std::vector<PowerUp> _powerUps;
    Background _background;
    Label _scoreLabel;
    Label _deadLabel;
    Label _pauseLabel;
    Label _gameOverLabel;
    Camera2D _camera;
    BackgroundRenderer _backgroundRenderer;
    FlappyRenderer _flappyRenderer;
    PillarRenderer _pillarRenderer;
    LabelRenderer _labelRenderer;
    PowerUpRenderer _powerUpRenderer;
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
