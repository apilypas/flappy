#include <cmath>
#include <ctime>
#include <vector>
#include "Constants.hpp"
#include "Game.hpp"
#include "FlappyRenderer.hpp"
#include "PillarRenderer.hpp"
#include "BackgroundRenderer.hpp"
#include "LabelRenderer.hpp"
#include "SfxPlayer.hpp"
#include "Entities.hpp"

void Game::Reset(Flappy &flappy, std::vector<Pillar> &pillars, GameState &gameState)
{
    flappy.rect.x = 100.0f;
    flappy.rect.y = 100.0f;
    flappy.rect.width = 32.0f;
    flappy.rect.height = 32.0f;
    flappy.velocity = 0.0f;
    flappy.rotation = 0.0f;
    flappy.rotationVelocity = 0.0f;
    flappy.speed = INITIAL_SPEED;

    pillars.clear();

    for (float i = 0, x = 300, y = 160; i < TOTAL_PILLARS; i++)
    {
        auto pillar = CreatePillar(x, y);

        x += PILLAR_SPACE;

        if (i < 5)
            y += 50;
        else
            y = (float)GetRandomValue(120, SCREEN_HEIGHT - 20);

        pillars.push_back(pillar);
    }

    gameState.score = 0;
}

Pillar Game::CreatePillar(float x, float y)
{
    Pillar pillar;

    pillar.bottom.x = x;
    pillar.bottom.y = y;
    pillar.bottom.width = PILLAR_WIDTH;
    pillar.bottom.height = PILLAR_HEIGHT;

    pillar.top.x = x;
    pillar.top.y = y - PILLAR_HEIGHT - PILLAR_GAP;
    pillar.top.width = PILLAR_WIDTH;
    pillar.top.height = PILLAR_HEIGHT;

    pillar.isScored = false;

    return pillar;
}

void Game::UpdatePhysics(Flappy &flappy)
{
    // Apply gravity
    flappy.velocity -= 0.1f;

    // Calculate y
    flappy.rect.y -= flappy.velocity;

    if (flappy.rotationVelocity > 0)
    {
        flappy.rotationVelocity -= 1.0f;
        flappy.rotation += flappy.rotationVelocity;
        if (flappy.rotation > 360.0f)
            flappy.rotation -= 360.0f;
    }
}

void Game::UpdatePillars(std::vector<Pillar> &pillars, Flappy &flappy, float scrollBy)
{
    bool doShift = pillars[0].bottom.x < flappy.rect.x - (160 * 6);

    if (doShift)
    {
        pillars.erase(pillars.begin());

        float x = pillars[pillars.size() - 1].top.x + PILLAR_SPACE;
        float y = (float)GetRandomValue(20 + PILLAR_GAP, SCREEN_HEIGHT - 20);
        
        auto pillar = CreatePillar(x, y);
        
        pillars.push_back(pillar);
    }

    for (auto &pillar : pillars)
    {
        pillar.top.x -= scrollBy;
        pillar.bottom.x -= scrollBy;
    }
}

bool Game::HandleScore(std::vector<Pillar> &pillars, Flappy &flappy, GameState &gameState)
{
    for (auto &pillar : pillars)
    {
        if (pillar.bottom.x < flappy.rect.x && !pillar.isScored)
        {
            pillar.isScored = true;
            gameState.score++;

            return true;
        }
    }

    return false;
}

void Game::UpdateBackground(Background &background, float scrollBy)
{
    background.parallaxX -= scrollBy / 2.0f;
    if (background.parallaxX <= -128.0f)
        background.parallaxX += 128.0f;
}

void Game::UpdateBanners(std::vector<Label> &banners, float scrollBy)
{
    for (auto &banner : banners)
    {
        banner.x -= scrollBy / 2.0f;
    }

    auto last = banners[banners.size() - 1];
    if (last.x < SCREEN_WIDTH * 2)
    {
        auto banner = CreateBanner(last.x + SCREEN_WIDTH, last.y);
        banners.push_back(banner);
    }
    
    if (banners[0].x <= -SCREEN_WIDTH * 2)
    {
        banners.erase(banners.begin());
    }
}

void Game::UpdateDeathState(Flappy &flappy, std::vector<Pillar> &pillars)
{
    Rectangle flappyHitBox = { 
        flappy.rect.x + 4, 
        flappy.rect.y + 4, 
        flappy.rect.width - 8, 
        flappy.rect.height - 8 
    };

    for (auto &pillar : pillars)
    {
        flappy.isDead = 
            CheckCollisionRecs(flappyHitBox, pillar.bottom)
            || CheckCollisionRecs(flappyHitBox, pillar.top)
            || flappy.rect.y > SCREEN_HEIGHT * 4;
        
        if (flappy.isDead)
            break;
    }
}

void Game::ShiftScreen(Flappy &flappy, Camera2D &camera, std::vector<Pillar> &pillars, Background &background, std::vector<Label> &banners)
{
    if (flappy.rect.x > SCREEN_WIDTH)
    {
        flappy.rect.x -= SCREEN_WIDTH;
        camera.target.x -= SCREEN_WIDTH;
        background.parallaxX -= SCREEN_WIDTH;
        
        for (auto &pillar : pillars)
        {
            pillar.bottom.x -= SCREEN_WIDTH;
            pillar.top.x -= SCREEN_WIDTH;
        }

        for (auto &banner : banners)
        {
            banner.x -= SCREEN_WIDTH;
        }
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

void Game::Initialize()
{
    SetRandomSeed((unsigned int)time(NULL));
    SetConfigFlags(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Oik");
    InitAudioDevice();
    SetExitKey(KEY_Q);
    SetTargetFPS(60);
}

void Game::Run() {
    bool showDebug = false;
    GameState gameState;
    Flappy flappy;
    std::vector<Pillar> pillars;
    std::vector<Label> banners;
    
    Background background;

    Label scoreLabel;
    scoreLabel.x = 10;
    scoreLabel.y = 10;
    scoreLabel.fontSize = 20;
    scoreLabel.color = YELLOW;
    scoreLabel.isVisible = true;

    Label deadLabel;
    deadLabel.x = 200;
    deadLabel.y = 200;
    deadLabel.fontSize = 40;
    deadLabel.color = RED;
    deadLabel.isVisible = false;

    Label pauseLabel;
    pauseLabel.text = "Press [SPACE] to unpause";
    pauseLabel.x = 200;
    pauseLabel.y = 200;
    pauseLabel.fontSize = 30;
    pauseLabel.color = WHITE;
    pauseLabel.isVisible = true;

    Label gameOverLabel;
    gameOverLabel.text = "Press [SPACE] to restart";
    gameOverLabel.x = 200;
    gameOverLabel.y = 200;
    gameOverLabel.fontSize = 30;
    gameOverLabel.color = WHITE;
    gameOverLabel.isVisible = false;

    banners.push_back(this->CreateBanner(200, 100));

    Camera2D camera;
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.target = { roundf(flappy.rect.x), roundf((float)SCREEN_HEIGHT / 2) };
    camera.offset = { 100.0f, SCREEN_HEIGHT / 2.0f };

    this->Reset(flappy, pillars, gameState);

    BackgroundRenderer backgroundRenderer;
    FlappyRenderer flappyRenderer;
    PillarRenderer pillarRenderer;
    LabelRenderer labelRenderer;
    SfxPlayer sfxPlayer;

    backgroundRenderer.Initialize();
    flappyRenderer.Initialize();
    pillarRenderer.Initialize();
    labelRenderer.Initialize();
    sfxPlayer.Initialize();

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();
        float scrollByX = flappy.speed * deltaTime;

        // Handle inputs
        if (IsKeyPressed(KEY_SPACE)
            || IsKeyPressed(KEY_UP)
            || IsKeyPressed(KEY_W)
            || IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            flappy.isJumping = true;
            gameState.isPaused = gameState.deathTimer > 0;
        }

        if (IsKeyPressed(KEY_ESCAPE))
        {
            gameState.isPaused = true;
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
        if (!gameState.isPaused && gameState.deathTimer <= 0)
        {
            if (flappy.isDead)
            {
                flappy.isDead = false;
                this->Reset(flappy, pillars, gameState);
            }

            this->UpdatePhysics(flappy);
            this->UpdatePillars(pillars, flappy, scrollByX);
            
            if (this->HandleScore(pillars, flappy, gameState))
                sfxPlayer.Play(SfxType::Point);

            if (flappy.isJumping)
            {
                flappy.velocity = JUMP_VELOCITY;
                flappy.rotationVelocity = 45.0f;
                sfxPlayer.Play(SfxType::Jump);
            }

            this->UpdateBackground(background, scrollByX);
            this->UpdateBanners(banners, scrollByX);

            this->UpdateDeathState(flappy, pillars);

            // Increase speed over time
            flappy.speed += deltaTime * 2.0f;

            if (flappy.isDead)
            {
                gameState.isPaused = true;
                gameState.deathTimer = 10.0f;
                sfxPlayer.Play(SfxType::Hit);
            }
        }

        if (gameState.deathTimer > 0)
            gameState.deathTimer -= deltaTime;

        // Fix for webassembly cam jigering
        this->ShiftScreen(flappy, camera, pillars, background, banners);

        // Reset values
        flappy.isJumping = false;

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

        camera.zoom = scale;

        camera.offset = {
            100.0f + ((float)screenWidth - SCREEN_WIDTH * scale) / 2.0f,
            (SCREEN_HEIGHT * scale) / 2.0f + ((float)screenHeight - SCREEN_HEIGHT * scale) / 2.0f
        };

        // Update labels
        scoreLabel.fontSize = 20.0f * scale;
        scoreLabel.x = (float)screenWidth / 2.0f - (float)MeasureText(scoreLabel.text, (int)scoreLabel.fontSize) / 2;
        scoreLabel.text = TextFormat("Score: %d", gameState.score);

        deadLabel.fontSize = 40.0f * scale;
        deadLabel.x = (float)screenWidth / 2.0f - (float)MeasureText(deadLabel.text, (int)deadLabel.fontSize) / 2;
        deadLabel.y = (float)screenHeight / 2.0f;
        deadLabel.isVisible = gameState.deathTimer > 0;
        deadLabel.text = TextFormat("You are DEAD!!! (%d)", (int)gameState.deathTimer);

        pauseLabel.fontSize = 30.0f * scale;
        pauseLabel.x = (float)screenWidth / 2.0f - (float)MeasureText(pauseLabel.text, (int)pauseLabel.fontSize) / 2;
        pauseLabel.y = (float)screenHeight / 2.0f;
        pauseLabel.isVisible = gameState.isPaused && !flappy.isDead;

        gameOverLabel.fontSize = 30.0f * scale;
        gameOverLabel.x = (float)screenWidth / 2.0f - (float)MeasureText(gameOverLabel.text, (int)gameOverLabel.fontSize) / 2;
        gameOverLabel.y = (float)screenHeight / 2.0f;
        gameOverLabel.isVisible = gameState.isPaused && flappy.isDead && gameState.deathTimer <= 0;

        BeginDrawing();

        ClearBackground({ 0xAD, 0xCA, 0xFF, 0xFF });

        BeginMode2D(camera);

        // Render background and background objects
        backgroundRenderer.Render(background);
        
        for (auto &banner : banners)
            labelRenderer.Render(banner);

        // Render world objects
        pillarRenderer.Render(pillars);
        flappyRenderer.Render(flappy);

        EndMode2D();

        // Render HUD
        labelRenderer.Render(scoreLabel);
        labelRenderer.Render(deadLabel);
        labelRenderer.Render(pauseLabel);
        labelRenderer.Render(gameOverLabel);

        if (showDebug)
        {
            DrawFPS(10, 10);
        }

        EndDrawing();
    }

    labelRenderer.Uninitialize();
    backgroundRenderer.Uninitialize();
    pillarRenderer.Uninitialize();
    flappyRenderer.Uninitialize();
    sfxPlayer.Uninitialize();
}

void Game::Uninitialize()
{
    CloseAudioDevice();
    CloseWindow();
}
