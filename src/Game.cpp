#include "Game.hpp"
#include "Constants.hpp"
#include "Entities.hpp"
#include "SfxPlayer.hpp"
#include <algorithm>
#include <cmath>
#include <ctime>
#include <raylib.h>
#include <vector>

void Game::Initialize()
{
    SetRandomSeed((unsigned int)time(NULL));
    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_RESIZABLE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Oik");
    InitAudioDevice();

    _showDebug = false;

    _gameBanner.text = "Press SPACE to start";
    _gameBanner.bannerText = "Flappy Oik";
    _gameBanner.authorText = "by g1ngercat.itch.io";
    _gameBanner.isVisible = true;

    _scoreLabel.x = 10;
    _scoreLabel.y = 10;
    _scoreLabel.fontSize = 20;
    _scoreLabel.color = {0xFF, 0xC9, 0x00, 0xFF};
    _scoreLabel.isVisible = true;

    _camera.zoom = 1.0f;
    _camera.rotation = 0.0f;
    _camera.target = {0.0f, 0.0f};
    _camera.offset = {0.0f, 0.0f};

    _backgroundRenderer.Initialize();
    _flappyRenderer.Initialize();
    _pillarRenderer.Initialize();
    _labelRenderer.Initialize();
    _powerUpRenderer.Initialize();
    _sfxPlayer.Initialize();
    _gameBannerRenderer.Initialize();
    _tooltipRenderer.Initialize();
    _musicPlayer.Initialize();

    _musicPlayer.Play();
    this->Reset();
}

void Game::DoFrame()
{
    float deltaTime = GetFrameTime();
    float scrollByX = _flappy.speed * deltaTime;

    // Handle inputs
    if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_UP) ||
        IsKeyPressed(KEY_W) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        _flappy.isJumping = true;
        _gameState.isPaused = _gameState.deathTimer > 0;
    }

    if (IsKeyPressed(KEY_ESCAPE)) {
        _gameState.isPaused = true;
    }

    if ((IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER)) ||
        (IsKeyDown(KEY_RIGHT_ALT) && IsKeyPressed(KEY_ENTER))) {
        ToggleFullscreen();
    }

    if (IsKeyPressed(KEY_F12)) {
        _showDebug = !_showDebug;
    }

    // Loop part that should be paused
    if (!_gameState.isPaused && _gameState.deathTimer <= 0) {
        if (_flappy.isDead) {
            _flappy.isDead = false;
            this->Reset();
        }

        this->UpdatePhysics(deltaTime);
        this->UpdatePillars(scrollByX);
        this->UpdatePowerUps(scrollByX);

        if (_flappy.isJumping) {
            _flappy.velocityY = -JUMP_VELOCITY;
            _flappy.rotationVelocity = ROTATION_VELOCITY;
            _sfxPlayer.Play(SfxType::Jump);
        }

        this->UpdateBackground(scrollByX);
        this->UpdateBanners(scrollByX);
        this->UpdateTooltips(scrollByX);

        this->HandleDeathState();

        if (this->HandleScore())
            _sfxPlayer.Play(SfxType::Point);

        if (this->HandlePowerUps())
            _sfxPlayer.Play(SfxType::PowerUp);

        if (this->HandleDoorUnlocks())
            _sfxPlayer.Play(SfxType::Unlock);

        // Increase speed over time
        _flappy.speed += deltaTime * 4.0f;

        // Update hi score
        if (_gameState.score > _gameState.hiScore)
            _gameState.hiScore = _gameState.score;

        if (_flappy.isDead) {
            _gameState.isPaused = true;
            _gameState.deathTimer = 1.0f;
            _sfxPlayer.Play(SfxType::Hit);
        }
    }

    if (_gameState.deathTimer > 0)
        _gameState.deathTimer -= deltaTime;

    // Reset values
    _flappy.isJumping = false;

    int screenWidth = GetRenderWidth();
    int screenHeight = GetRenderHeight();

    if (screenWidth < SCREEN_WIDTH || screenHeight < SCREEN_HEIGHT) {
        SetWindowSize(screenWidth < SCREEN_WIDTH ? SCREEN_WIDTH : screenWidth,
                      screenHeight < SCREEN_HEIGHT ? SCREEN_HEIGHT
                                                   : screenHeight);
    }

    float scaleX = (float)screenWidth / SCREEN_WIDTH;
    float scaleY = (float)screenHeight / SCREEN_HEIGHT;
    float scale = fminf(scaleX, scaleY);

    _camera.zoom = scale;

    // Adjust offset to correctly fit everything in screen
    _camera.offset = {((float)screenWidth - (float)SCREEN_WIDTH * scale) / 2.0f,
                      ((float)screenHeight - (float)SCREEN_HEIGHT * scale) /
                          2.0f};

    // Update labels
    _scoreLabel.fontSize = 20.0f * scale;
    _scoreLabel.x =
        (float)screenWidth / 2.0f -
        (float)MeasureText(_scoreLabel.text, (int)_scoreLabel.fontSize) / 2;
    _scoreLabel.text = TextFormat("Score: %d", _gameState.score);
    _gameBanner.gameOverText = "";

    if (_flappy.isDead) {
        _gameBanner.gameOverText = TextFormat("Game is OVER!");
        _gameBanner.text = "Press SPACE to restart";
    } else if (_gameState.isPaused) {
        _gameBanner.text = "Press SPACE to unpause";
    }

    if (_gameState.isPaused) {
        _gameBanner.hiScoreText =
            TextFormat("HI SCORE: %d", _gameState.hiScore);
        _musicPlayer.MakeVolumeLower();
    } else {
        _musicPlayer.MakeVolumeHigher();
    }

    _gameBanner.scale = scale;
    _gameBanner.isVisible = _gameState.isPaused;
    _gameBanner.color = WHITE;

    _musicPlayer.Update();

    BeginDrawing();

    ClearBackground(BLACK);

    BeginMode2D(_camera);

    // Render background and background objects
    _backgroundRenderer.Render(_background);

    for (Label &banner : _banners)
        _labelRenderer.Render(banner);

    // Render world objects
    for (Pillar &pillar : _pillars)
        _pillarRenderer.Render(pillar);

    for (PowerUp &powerUp : _powerUps)
        _powerUpRenderer.Render(powerUp);

    for (Tooltip &tooltip : _tooltips)
        _tooltipRenderer.Render(tooltip);

    _flappyRenderer.Render(_flappy);

    EndMode2D();

    // Render HUD
    _labelRenderer.Render(_scoreLabel);
    _gameBannerRenderer.Render(_gameBanner);

    if (_showDebug) {
        DrawFPS(10, 10);
    }

    EndDrawing();
}

void Game::Uninitialize()
{
    _labelRenderer.Uninitialize();
    _backgroundRenderer.Uninitialize();
    _pillarRenderer.Uninitialize();
    _flappyRenderer.Uninitialize();
    _powerUpRenderer.Uninitialize();
    _sfxPlayer.Uninitialize();
    _gameBannerRenderer.Uninitialize();
    _tooltipRenderer.Uninitialize();
    _musicPlayer.Uninitialize();

    CloseAudioDevice();
    CloseWindow();
}

void Game::Reset()
{
    _flappy.radius = 16.0f;
    _flappy.center.x = 116.0f;
    _flappy.center.y = SCREEN_HEIGHT / 2.0f;
    _flappy.velocityY = 0.0f;
    _flappy.rotation = 0.0f;
    _flappy.rotationVelocity = 0.0f;
    _flappy.speed = INITIAL_SPEED;

    _banners.clear();
    _pillars.clear();
    _powerUps.clear();
    _tooltips.clear();
    _powerUpCounts.clear();

    _nextPillarId = 0;
    _nextTooltipId = 0;

    for (float i = 0, x = 300, gap = PILLAR_GAP, y = 20 + gap;
         i < TOTAL_PILLARS;
         i++) {
        Pillar pillar = CreatePillar(x, y);

        gap = pillar.gapSize;

        x += PILLAR_SPACE;

        if (i < 3)
            y += 50;
        else
            y = (float)GetRandomValue(20 + (int)pillar.gapSize,
                                      SCREEN_HEIGHT - 20);

        _pillars.push_back(pillar);
    }

    _gameState.score = 0;

    this->UpdateBanners(0.0f);
    this->UpdatePillars(0.0f);
    this->UpdatePowerUps(0.0f);

    TraceLog(LOG_INFO, "Game reset");
}

Pillar Game::CreatePillar(float x, float y)
{
    Pillar pillar;

    pillar.id = _nextPillarId++;
    pillar.type = PillarType::Normal;
    pillar.gapSize = PILLAR_GAP;

    if ((_nextPillarId <= 20 && _nextPillarId % 5 == 0) ||
        (_nextPillarId > 20 && GetRandomValue(0, 2) == 0)) {
        pillar.type = PillarType::Sliding;
        pillar.isSlidingUp = (bool)GetRandomValue(0, 1);
        pillar.slidingSpeed = 60.0f + (float)GetRandomValue(-30, 30);
        pillar.hasPowerUp = true;
    }

    if (pillar.type == PillarType::Normal && pillar.id >= 10 &&
        GetRandomValue(0, 1))
        pillar.gapSize = PILLAR_GAP_SMALL;

    pillar.bottom.x = x;
    pillar.bottom.y = y;
    pillar.bottom.width = PILLAR_WIDTH;
    pillar.bottom.height = PILLAR_HEIGHT;

    pillar.top.x = x;
    pillar.top.y = y - PILLAR_HEIGHT - pillar.gapSize;
    pillar.top.width = PILLAR_WIDTH;
    pillar.top.height = PILLAR_HEIGHT;

    if (_nextPillarId > 10 && GetRandomValue(0, 1) == 0) {
        pillar.door.x = x;
        pillar.door.y = y - pillar.gapSize;
        pillar.door.width = PILLAR_WIDTH;
        pillar.door.height = pillar.gapSize;

        pillar.lockCenter = {
            x - 48.0f,
            y + (float)GetRandomValue(0, (int)(-pillar.gapSize + 32.0f))};
        pillar.lockRadius = 16.0f;

        pillar.isLocked = true;
    }

    pillar.isScored = false;

    CreateTooltip(pillar);

    TraceLog(LOG_INFO, "Created pillar: id=%d", pillar.id);

    return pillar;
}

void Game::UpdatePhysics(float deltaTime)
{
    // Apply gravity
    _flappy.velocityY += 980.0f * deltaTime;

    // Calculate y
    _flappy.center.y += _flappy.velocityY * deltaTime;

    if (_flappy.rotationVelocity > 0) {
        _flappy.rotationVelocity -= 2000.0f * deltaTime;
        _flappy.rotationVelocity = std::max(0.0f, _flappy.rotationVelocity);
        _flappy.rotation += _flappy.rotationVelocity * deltaTime;
        if (_flappy.rotation > 360.0f)
            _flappy.rotation -= 360.0f;
    }
}

void Game::UpdatePillars(float scrollBy)
{
    bool doShift = _pillars[0].bottom.x < _flappy.center.x - (160 * 7);
    float deltaTime = GetFrameTime();

    if (doShift) {
        _pillars.erase(_pillars.begin());

        float x = _pillars[_pillars.size() - 1].top.x + PILLAR_SPACE;
        float y = (float)GetRandomValue(
            40 + (int)(_pillars[_pillars.size() - 1].gapSize),
            SCREEN_HEIGHT - 40);

        Pillar pillar = CreatePillar(x, y);

        _pillars.push_back(pillar);
    }

    for (Pillar &pillar : _pillars) {
        if (pillar.type == PillarType::Sliding) {
            if (pillar.isSlidingUp) {
                pillar.bottom.y -= pillar.slidingSpeed * deltaTime;
                pillar.top.y -= pillar.slidingSpeed * deltaTime;
                pillar.door.y -= pillar.slidingSpeed * deltaTime;
                pillar.lockCenter.y -= pillar.slidingSpeed * deltaTime;
                if (pillar.bottom.y <= 20.0f + pillar.gapSize)
                    pillar.isSlidingUp = false;
            } else {
                pillar.bottom.y += pillar.slidingSpeed * deltaTime;
                pillar.top.y += pillar.slidingSpeed * deltaTime;
                pillar.door.y += pillar.slidingSpeed * deltaTime;
                pillar.lockCenter.y += pillar.slidingSpeed * deltaTime;
                if (pillar.bottom.y >= SCREEN_HEIGHT - 20.0f)
                    pillar.isSlidingUp = true;
            }
        }
    }

    // Scroll pillars
    for (Pillar &pillar : _pillars) {
        pillar.top.x -= scrollBy;
        pillar.bottom.x -= scrollBy;
        pillar.door.x -= scrollBy;
        pillar.lockCenter.x -= scrollBy;
    }

    for (Pillar &pillar : _pillars) {
        if (pillar.tooltipId > 0 && !pillar.isLocked) {
            for (Tooltip &tooltip : _tooltips) {
                if (tooltip.id == pillar.tooltipId)
                    tooltip.text = "Unlocked";
            }
            pillar.tooltipId = 0;
        }
    }
}

void Game::UpdatePowerUps(float scrollBy)
{
    for (auto it = _powerUps.begin(); it != _powerUps.end();) {
        it->rect.x -= scrollBy;

        // Delete when too far
        if (it->rect.x < -SCREEN_WIDTH * 2)
            it = _powerUps.erase(it);
        else
            it++;
    }

    for (Pillar &pillar : _pillars) {
        if (!pillar.hasPowerUp) {
            int slowChanceMax = 2;
            if (pillar.id > 30)
                slowChanceMax = 3;
            if (pillar.id > 80)
                slowChanceMax = 4;
            if (pillar.id > 160)
                slowChanceMax = 6;
            if (pillar.id > 300)
                slowChanceMax = 8;

            int slowChance = GetRandomValue(0, slowChanceMax);

            if (slowChance == 0) {
                PowerUp powerUp;
                powerUp.type = PowerUpType::Slow;
                powerUp.rect = {pillar.bottom.x,
                                (float)GetRandomValue(
                                    (int)pillar.top.y + (int)pillar.top.height,
                                    (int)pillar.bottom.y - (int)40.0f),
                                32,
                                32};

                if (pillar.isLocked) {
                    powerUp.rect.x =
                        pillar.bottom.x + pillar.bottom.width + 16.0f;
                    powerUp.rect.y =
                        pillar.bottom.y - pillar.gapSize / 2.0f - 16.0f;
                }

                this->CreateTooltip(powerUp);

                _powerUps.push_back(powerUp);
            }

            int pointsChance = GetRandomValue(0, 2);

            if (pointsChance == 0) {
                bool isOnTop = GetRandomValue(0, 1);
                PowerUp powerUp;
                powerUp.type = PowerUpType::Points;
                powerUp.rect = {
                    pillar.bottom.x +
                        (float)GetRandomValue(60, PILLAR_SPACE - 60),
                    isOnTop ? 20.0f : SCREEN_HEIGHT - 32.0f - 20.0f,
                    32,
                    32};

                this->CreateTooltip(powerUp);

                _powerUps.push_back(powerUp);
            }

            pillar.hasPowerUp = true;
        }
    }
}

bool Game::HandleScore()
{
    for (Pillar &pillar : _pillars) {
        if (pillar.bottom.x < _flappy.center.x && !pillar.isScored) {
            pillar.isScored = true;
            _gameState.score++;

            return true;
        }
    }

    return false;
}

void Game::UpdateBackground(float scrollBy)
{
    _background.parallaxX -= scrollBy / 2.0f;
    if (_background.parallaxX <= -128.0f)
        _background.parallaxX += 128.0f;
}

void Game::UpdateBanners(float scrollBy)
{
    for (Label &banner : _banners) {
        banner.x -= scrollBy / 2.0f;
    }

    if (_banners.size() <= 0) {
        Label banner = CreateBanner(300.0f, 100.0f);
        _banners.push_back(banner);
    }

    Label last = _banners[_banners.size() - 1];
    if (last.x < SCREEN_WIDTH * 2) {
        Label banner = CreateBanner(last.x + SCREEN_WIDTH, last.y);
        _banners.push_back(banner);
    }

    if (_banners[0].x <= -SCREEN_WIDTH * 2) {
        _banners.erase(_banners.begin());
    }
}

void Game::HandleDeathState()
{
    for (Pillar &pillar : _pillars) {
        _flappy.isDead = CheckCollisionCircleRec(_flappy.center,
                                                 _flappy.radius - 4.0f,
                                                 pillar.bottom) ||
                         CheckCollisionCircleRec(_flappy.center,
                                                 _flappy.radius - 4.0f,
                                                 pillar.top) ||
                         (pillar.door.width > 0.01f &&
                          pillar.door.height >= 0.01f && pillar.isLocked &&
                          CheckCollisionCircleRec(_flappy.center,
                                                  _flappy.radius - 4.0f,
                                                  pillar.door)) ||
                         _flappy.center.y > SCREEN_HEIGHT * 4;

        if (_flappy.isDead)
            break;
    }
}

Label Game::CreateBanner(float x, float y)
{
    Label banner;

    banner.x = x;
    banner.y = y;
    banner.rotation = (float)GetRandomValue(0, 180);
    banner.fontSize = 40;
    banner.color = {0x4C, 0x85, 0xE3, 0xFF};
    banner.shadowColor = {0x19, 0x50, 0xA7, 0xFF};
    banner.isVisible = true;
    banner.rotation = (float)GetRandomValue(0, 180);
    banner.text = _bannerMessageSource.GetRandom();

    int colorChoice = GetRandomValue(0, 2);

    if (colorChoice == 1) {
        banner.color = {0xDF, 0x46, 0x38, 0xFF};
        banner.shadowColor = {0xAB, 0x26, 0x1A, 0xFF};
    }

    if (colorChoice == 2) {
        banner.color = {0xDF, 0xC0, 0x4D, 0xFF};
        banner.shadowColor = {0xBB, 0x9D, 0x2D, 0xFF};
    }

    return banner;
}

bool Game::HandlePowerUps()
{
    bool isTaken = false;

    for (auto it = _powerUps.begin(); it != _powerUps.end();) {
        if (CheckCollisionCircleRec(_flappy.center, _flappy.radius, it->rect)) {
            if (it->type == PowerUpType::Slow) {
                _flappy.speed =
                    std::max(_flappy.speed - _flappy.speed * 0.2f, 10.0f);
            } else if (it->type == PowerUpType::Points) {
                _gameState.score += 10;
            }

            if (it->tooltipId > 0) {
                for (auto tooltipIt = _tooltips.begin();
                     tooltipIt != _tooltips.end();) {
                    if (tooltipIt->id == it->tooltipId)
                        _tooltips.erase(tooltipIt);
                    else
                        tooltipIt++;
                }
            }

            it = _powerUps.erase(it);
            isTaken = true;
        } else
            it++;
    }

    return isTaken;
}

bool Game::HandleDoorUnlocks()
{
    bool isUnlocked = false;

    // Handle door locks
    for (Pillar &pillar : _pillars) {
        if (pillar.door.height > 0.01f && pillar.door.width > 0.01f) {
            if (pillar.isLocked && CheckCollisionCircles(pillar.lockCenter,
                                                         pillar.lockRadius,
                                                         _flappy.center,
                                                         _flappy.radius)) {
                // Open door
                isUnlocked = true;
                pillar.isLocked = false;
            }
        }
    }

    return isUnlocked;
}

void Game::UpdateTooltips(float scrollBy)
{
    for (auto it = _tooltips.begin(); it != _tooltips.end();) {
        it->x -= scrollBy;

        // Delete when too far
        if (it->x < -SCREEN_WIDTH * 2)
            it = _tooltips.erase(it);
        else
            it++;
    }
}

void Game::CreateTooltip(PowerUp &powerUp)
{
    if (_powerUpCounts.find(powerUp.type) == _powerUpCounts.end())
        _powerUpCounts[powerUp.type] = 0;

    _powerUpCounts[powerUp.type]++;

    // Limit amount of tooltips displayed to avoid clutter
    if (_powerUpCounts[powerUp.type] > 2)
        return;

    Tooltip tooltip;

    _nextTooltipId++;
    tooltip.id = _nextTooltipId;
    tooltip.text = "Unkown?";

    if (powerUp.type == PowerUpType::Slow)
        tooltip.text = "Slows down";
    if (powerUp.type == PowerUpType::Points)
        tooltip.text = "Extra points";

    tooltip.x = powerUp.rect.x + powerUp.rect.width / 2.0f;
    tooltip.y = powerUp.rect.y - 16.0f;

    if (tooltip.y < 64.0f)
        tooltip.y += 58.0f;

    powerUp.tooltipId = tooltip.id;

    _tooltips.push_back(tooltip);
}

void Game::CreateTooltip(Pillar &pillar)
{
    if (pillar.door.width > 0.01f && pillar.door.height > 0.01 &&
        pillar.type == PillarType::Normal) {
        if (_powerUpCounts.find(PowerUpType::Unlock) == _powerUpCounts.end())
            _powerUpCounts[PowerUpType::Unlock] = 0;

        _powerUpCounts[PowerUpType::Unlock]++;

        // Limit amount of tooltips displayed to avoid clutter
        if (_powerUpCounts[PowerUpType::Unlock] > 2)
            return;

        Tooltip tooltip;

        _nextTooltipId++;
        tooltip.id = _nextTooltipId;
        tooltip.text = "Locked";

        tooltip.x = pillar.lockCenter.x;
        tooltip.y = pillar.lockCenter.y - 32.0f;

        if (tooltip.y < 64.0f)
            tooltip.y += 58.0f;

        pillar.tooltipId = tooltip.id;

        _tooltips.push_back(tooltip);
    }
}
