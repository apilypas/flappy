#include <cmath>
#include <ctime>
#include <raylib.h>
#include <vector>
#include "Constants.hpp"
#include "Game.hpp"
#include "Entities.hpp"

void Game::Initialize()
{
    SetRandomSeed((unsigned int)time(NULL));
    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Oik");
    InitAudioDevice();
    SetExitKey(KEY_Q);
    SetTargetFPS(60);
}

void Game::Run()
{
    showDebug = false;

    _scoreLabel.x = 10;
    _scoreLabel.y = 10;
    _scoreLabel.fontSize = 20;
    _scoreLabel.color = YELLOW;
    _scoreLabel.isVisible = true;

    _deadLabel.x = 200;
    _deadLabel.y = 200;
    _deadLabel.fontSize = 40;
    _deadLabel.color = RED;
    _deadLabel.isVisible = false;

    _pauseLabel.text = "Press [SPACE] to unpause";
    _pauseLabel.x = 200;
    _pauseLabel.y = 200;
    _pauseLabel.fontSize = 30;
    _pauseLabel.color = WHITE;
    _pauseLabel.isVisible = true;

    _gameOverLabel.text = "Press [SPACE] to restart";
    _gameOverLabel.x = 200;
    _gameOverLabel.y = 200;
    _gameOverLabel.fontSize = 30;
    _gameOverLabel.color = WHITE;
    _gameOverLabel.isVisible = false;

    _camera.zoom = 1.0f;
    _camera.rotation = 0.0f;
    _camera.target = { 0.0f, 0.0f };
    _camera.offset = { 0.0f, 0.0f };

    _backgroundRenderer.Initialize();
    _flappyRenderer.Initialize();
    _pillarRenderer.Initialize();
    _labelRenderer.Initialize();
    _powerUpRenderer.Initialize();
    _sfxPlayer.Initialize();

    this->Reset();

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        float scrollByX = _flappy.speed * deltaTime;

        // Handle inputs
        if (IsKeyPressed(KEY_SPACE)
            || IsKeyPressed(KEY_UP)
            || IsKeyPressed(KEY_W)
            || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            _flappy.isJumping = true;
            _gameState.isPaused = _gameState.deathTimer > 0;
        }

        if (IsKeyPressed(KEY_ESCAPE))
        {
            _gameState.isPaused = true;
        }

        if ((IsKeyDown(KEY_LEFT_ALT) && IsKeyPressed(KEY_ENTER))
            || (IsKeyDown(KEY_RIGHT_ALT) && IsKeyPressed(KEY_ENTER)))
        {
            ToggleFullscreen();
        }

        if (IsKeyPressed(KEY_F12))
        {
            showDebug = !showDebug;
        }

        // Loop part that should be paused
        if (!_gameState.isPaused && _gameState.deathTimer <= 0)
        {
            if (_flappy.isDead)
            {
                _flappy.isDead = false;
                this->Reset();
            }

            this->UpdatePhysics();
            this->UpdatePillars(scrollByX);
            this->UpdatePowerUps(scrollByX);

            if (_flappy.isJumping)
            {
                _flappy.velocity = JUMP_VELOCITY;
                _flappy.rotationVelocity = 45.0f;
                _sfxPlayer.Play(SfxType::Jump);
            }

            this->UpdateBackground(scrollByX);
            this->UpdateBanners(scrollByX);

            this->HandleDeathState();

            if (this->HandleScore())
                _sfxPlayer.Play(SfxType::Point);

            if (this->HandlePowerUps())
                _sfxPlayer.Play(SfxType::PowerUp);

            // Increase speed over time
            _flappy.speed += deltaTime * 2.0f;

            if (_flappy.isDead)
            {
                _gameState.isPaused = true;
                _gameState.deathTimer = 10.0f;
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
            SetWindowSize(
                screenWidth < SCREEN_WIDTH ? SCREEN_WIDTH : screenWidth,
                screenHeight < SCREEN_HEIGHT ? SCREEN_HEIGHT : screenHeight
            );
        }

        float scaleX = (float)screenWidth / SCREEN_WIDTH;
        float scaleY = (float)screenHeight / SCREEN_HEIGHT;
        float scale = fminf(scaleX, scaleY);

        _camera.zoom = scale;

        // Adjust offset to correctly fit everything in screen
        _camera.offset = {
            ((float)screenWidth - (float)SCREEN_WIDTH * scale) / 2.0f,
            ((float)screenHeight - (float)SCREEN_HEIGHT * scale) / 2.0f
        };

        // Update labels
        _scoreLabel.fontSize = 20.0f * scale;
        _scoreLabel.x = (float)screenWidth / 2.0f - (float)MeasureText(_scoreLabel.text, (int)_scoreLabel.fontSize) / 2;
        _scoreLabel.text = TextFormat("Score: %d", _gameState.score);

        _deadLabel.fontSize = 40.0f * scale;
        _deadLabel.x = (float)screenWidth / 2.0f - (float)MeasureText(_deadLabel.text, (int)_deadLabel.fontSize) / 2;
        _deadLabel.y = (float)screenHeight / 2.0f;
        _deadLabel.isVisible = _gameState.deathTimer > 0;
        _deadLabel.text = TextFormat("You are DEAD!!! (%d)", (int)_gameState.deathTimer);

        _pauseLabel.fontSize = 30.0f * scale;
        _pauseLabel.x = (float)screenWidth / 2.0f - (float)MeasureText(_pauseLabel.text, (int)_pauseLabel.fontSize) / 2;
        _pauseLabel.y = (float)screenHeight / 2.0f;
        _pauseLabel.isVisible = _gameState.isPaused && !_flappy.isDead;

        _gameOverLabel.fontSize = 30.0f * scale;
        _gameOverLabel.x = (float)screenWidth / 2.0f - (float)MeasureText(_gameOverLabel.text, (int)_gameOverLabel.fontSize) / 2;
        _gameOverLabel.y = (float)screenHeight / 2.0f;
        _gameOverLabel.isVisible = _gameState.isPaused && _flappy.isDead && _gameState.deathTimer <= 0;

        BeginDrawing();

        ClearBackground({ 0xAD, 0xCA, 0xFF, 0xFF });

        BeginMode2D(_camera);

        // Render background and background objects
        _backgroundRenderer.Render(_background);
        
        for (auto &banner : _banners)
            _labelRenderer.Render(banner);

        // Render world objects
        for (auto &pillar : _pillars)
            _pillarRenderer.Render(pillar);
        
        for (auto &powerUp : _powerUps)
            _powerUpRenderer.Render(powerUp);
        
        _flappyRenderer.Render(_flappy);

        EndMode2D();

        // Render HUD
        _labelRenderer.Render(_scoreLabel);
        _labelRenderer.Render(_deadLabel);
        _labelRenderer.Render(_pauseLabel);
        _labelRenderer.Render(_gameOverLabel);

        if (showDebug)
        {
            DrawFPS(10, 10);
        }

        EndDrawing();
    }

    _labelRenderer.Uninitialize();
    _backgroundRenderer.Uninitialize();
    _pillarRenderer.Uninitialize();
    _flappyRenderer.Uninitialize();
    _powerUpRenderer.Uninitialize();
    _sfxPlayer.Uninitialize();
}

void Game::Uninitialize()
{
    CloseAudioDevice();
    CloseWindow();
}

void Game::Reset()
{
    _flappy.rect.x = 100.0f;
    _flappy.rect.y = SCREEN_HEIGHT / 2.0f - 16.0f;
    _flappy.rect.width = 32.0f;
    _flappy.rect.height = 32.0f;
    _flappy.velocity = 0.0f;
    _flappy.rotation = 0.0f;
    _flappy.rotationVelocity = 0.0f;
    _flappy.speed = INITIAL_SPEED;

    _banners.clear();
    _pillars.clear();
    _powerUps.clear();
    _nextPillarId = 0;

    for (float i = 0, x = 300, y = 20 + PILLAR_GAP; i < TOTAL_PILLARS; i++)
    {
        auto pillar = CreatePillar(x, y);

        x += PILLAR_SPACE;

        if (i < 3)
            y += 50;
        else
            y = (float)GetRandomValue(20 + PILLAR_GAP, SCREEN_HEIGHT - 20);

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

    if (_nextPillarId % 5 == 0)
    {
        pillar.type = PillarType::Sliding;
        pillar.isSlidingUp = (bool)GetRandomValue(0, 1);
        pillar.slidingSpeed = 60.0f + (float)GetRandomValue(-20, 20);
        pillar.hasPowerUp = true;
    }

    pillar.bottom.x = x;
    pillar.bottom.y = y;
    pillar.bottom.width = PILLAR_WIDTH;
    pillar.bottom.height = PILLAR_HEIGHT;

    pillar.top.x = x;
    pillar.top.y = y - PILLAR_HEIGHT - PILLAR_GAP;
    pillar.top.width = PILLAR_WIDTH;
    pillar.top.height = PILLAR_HEIGHT;

    pillar.isScored = false;

    TraceLog(LOG_INFO, "Created pillar: id=%d", pillar.id);

    return pillar;
}

void Game::UpdatePhysics()
{
    // Apply gravity
    _flappy.velocity -= 0.1f;

    // Calculate y
    _flappy.rect.y -= _flappy.velocity;

    if (_flappy.rotationVelocity > 0)
    {
        _flappy.rotationVelocity -= 1.0f;
        _flappy.rotation += _flappy.rotationVelocity;
        if (_flappy.rotation > 360.0f)
            _flappy.rotation -= 360.0f;
    }
}

void Game::UpdatePillars(float scrollBy)
{
    bool doShift = _pillars[0].bottom.x < _flappy.rect.x - (160 * 7);
    float deltaTime = GetFrameTime();

    if (doShift)
    {
        _pillars.erase(_pillars.begin());

        float x = _pillars[_pillars.size() - 1].top.x + PILLAR_SPACE;
        float y = (float)GetRandomValue(40 + PILLAR_GAP, SCREEN_HEIGHT - 40);
        
        auto pillar = CreatePillar(x, y);
        
        _pillars.push_back(pillar);
    }

    for (auto &pillar: _pillars)
    {
        if (pillar.type == PillarType::Sliding)
        {
            if (pillar.isSlidingUp)
            {
                pillar.bottom.y -= pillar.slidingSpeed * deltaTime;
                pillar.top.y -= pillar.slidingSpeed * deltaTime;
                if (pillar.bottom.y <= 20.0f + PILLAR_GAP)
                    pillar.isSlidingUp = false;
            }
            else
            {
                pillar.bottom.y += pillar.slidingSpeed * deltaTime;
                pillar.top.y += pillar.slidingSpeed * deltaTime;
                if (pillar.bottom.y >= SCREEN_HEIGHT - 20.0f)
                    pillar.isSlidingUp = true;
            }
        }
    }

    for (auto &pillar : _pillars)
    {
        pillar.top.x -= scrollBy;
        pillar.bottom.x -= scrollBy;
    }
}

void Game::UpdatePowerUps(float scrollBy)
{
    for (auto it = _powerUps.begin(); it != _powerUps.end(); )
    {
        it->rect.x -= scrollBy;

        // Delete when too far
        if (it->rect.x < -SCREEN_WIDTH * 2)
            it = _powerUps.erase(it);
        else
            it++;
    }
    
    for (auto &pillar : _pillars)
    {
        if (!pillar.hasPowerUp)
        {
            int r = GetRandomValue(0, 5);
            
            if (r == 0)
            {
                PowerUp powerUp;
                powerUp.type = PowerUpType::Slow;
                powerUp.rect = { 
                    pillar.bottom.x,
                    pillar.bottom.y - 40.0f,
                    32,
                    32
                };
                _powerUps.push_back(powerUp);
            }

            if (r == 1)
            {
                PowerUp powerUp;
                powerUp.type = PowerUpType::Slow;
                powerUp.rect = { 
                    pillar.bottom.x,
                    pillar.bottom.y - PILLAR_GAP + 8.0f,
                    32,
                    32
                };
                _powerUps.push_back(powerUp);
            }

            if (r == 2)
            {
                PowerUp powerUp;
                powerUp.type = PowerUpType::Points;
                powerUp.rect = { 
                    pillar.bottom.x + (float)GetRandomValue(60, PILLAR_SPACE - 60),
                    20.0f,
                    32,
                    32
                };
                _powerUps.push_back(powerUp);
            }

            if (r == 3)
            {
                PowerUp powerUp;
                powerUp.type = PowerUpType::Points;
                powerUp.rect = { 
                    pillar.bottom.x + (float)GetRandomValue(60, PILLAR_SPACE - 60),
                    SCREEN_HEIGHT - 32.0f - 20.0f,
                    32,
                    32
                };
                _powerUps.push_back(powerUp);
            }
            
            pillar.hasPowerUp = true;
        }
    }
}

bool Game::HandleScore()
{
    for (auto &pillar : _pillars)
    {
        if (pillar.bottom.x < _flappy.rect.x && !pillar.isScored)
        {
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
    for (auto &banner : _banners)
    {
        banner.x -= scrollBy / 2.0f;
    }

    if (_banners.size() <= 0)
    {
        auto banner = CreateBanner(300.0f, 100.0f);
        _banners.push_back(banner);
    }

    auto last = _banners[_banners.size() - 1];
    if (last.x < SCREEN_WIDTH * 2)
    {
        auto banner = CreateBanner(last.x + SCREEN_WIDTH, last.y);
        _banners.push_back(banner);
    }
    
    if (_banners[0].x <= -SCREEN_WIDTH * 2)
    {
        _banners.erase(_banners.begin());
    }
}

void Game::HandleDeathState()
{
    Rectangle flappyHitBox = { 
        _flappy.rect.x + 4, 
        _flappy.rect.y + 4, 
        _flappy.rect.width - 8, 
        _flappy.rect.height - 8 
    };

    for (auto &pillar : _pillars)
    {
        _flappy.isDead = 
            CheckCollisionRecs(flappyHitBox, pillar.bottom)
            || CheckCollisionRecs(flappyHitBox, pillar.top)
            || _flappy.rect.y > SCREEN_HEIGHT * 4;
        
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
    banner.color = SKYBLUE;
    banner.shadowColor = BLUE;
    banner.isVisible = true;
    banner.rotation = (float)GetRandomValue(0, 180);
    banner.text = _bannerMessageSource.GetRandom();

    int colorChoice = GetRandomValue(0, 2);

    if (colorChoice == 1)
    {
        banner.color = PINK;
        banner.shadowColor = RED;
    }

    if (colorChoice == 2)
    {
        banner.color = { 0xff, 0xfc, 0xb4, 255 };
        banner.shadowColor = ORANGE;
    }

    return banner;
}

bool Game::HandlePowerUps()
{
    bool isTaken = false;

    for (auto it = _powerUps.begin(); it != _powerUps.end(); )
    {
        if (CheckCollisionRecs(_flappy.rect, it->rect))
        {
            if (it->type == PowerUpType::Slow)
            {
                _flappy.speed = INITIAL_SPEED;
            }
            else if (it->type == PowerUpType::Points)
            {
                _gameState.score += 10;
            }

            it = _powerUps.erase(it);
            isTaken = true;
        }
        else
            it++;
    }

    return isTaken;
}
