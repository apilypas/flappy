#pragma once
#include <map>
#include <vector>
#include "Entities.hpp"
#include "BannerMessageSource.hpp"
#include "FlappyRenderer.hpp"
#include "MusicPlayer.hpp"
#include "PillarRenderer.hpp"
#include "BackgroundRenderer.hpp"
#include "LabelRenderer.hpp"
#include "PowerUpRenderer.hpp"
#include "GameBannerRenderer.hpp"
#include "SfxPlayer.hpp"
#include "TooltipRenderer.hpp"

class Game {
    bool _showDebug;
    int _nextPillarId = 0;
    int _nextTooltipId = 0;
    std::map<PowerUpType, int> _powerUpCounts;

    BannerMessageSource _bannerMessageSource;

    Camera2D _camera;
    
    GameState _gameState;
    Flappy _flappy;
    std::vector<Pillar> _pillars;
    std::vector<Label> _banners;
    std::vector<PowerUp> _powerUps;
    std::vector<Tooltip> _tooltips;
    Background _background;

    GameBanner _gameBanner;
    Label _scoreLabel;
    
    BackgroundRenderer _backgroundRenderer;
    FlappyRenderer _flappyRenderer;
    PillarRenderer _pillarRenderer;
    LabelRenderer _labelRenderer;
    PowerUpRenderer _powerUpRenderer;
    GameBannerRenderer _gameBannerRenderer;
    TooltipRenderer _tooltipRenderer;

    SfxPlayer _sfxPlayer;
    MusicPlayer _musicPlayer;
public:
    void Initialize();
    void DoFrame();
    void Uninitialize();
private:
    void Reset();
    Pillar CreatePillar(float, float);
    void UpdatePhysics();
    void UpdatePillars(float);
    bool HandleScore();
    void UpdateBackground(float);
    void UpdateBanners(float);
    void UpdatePowerUps(float);
    void HandleDeathState();
    bool HandlePowerUps();
    bool HandleDoorUnlocks();
    Label CreateBanner(float, float);
    void UpdateTooltips(float);
    void CreateTooltip(PowerUp&);
    void CreateTooltip(Pillar&);
};
