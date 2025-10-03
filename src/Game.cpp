#include <cmath>
#include <ctime>
#include "Constants.hpp"
#include "Game.hpp"
#include "FlappyRenderer.hpp"
#include "PillarRenderer.hpp"
#include "BackgroundRenderer.hpp"
#include "LabelRenderer.hpp"
#include "SfxPlayer.hpp"
#include "Entities.hpp"

const int TotalWidth = 30;
const int PillarSpace = 200;
const int PillarGap = 120;
const int JumpVelocity = 3;

void Game::Reset(Flappy &flappy, Pillar *pillars, Camera2D &camera, GameState &gameState)
{
    flappy.rect.x = 100.0f;
    flappy.rect.y = 100.0f;
    flappy.rect.width = 32.0f;
    flappy.rect.height = 32.0f;
    flappy.velocity = 0.0f;
    flappy.rotation = 0.0f;
    flappy.rotationVelocity = 0.0f;

    for (int i = 0, x = 300, y = 140; i < TOTAL_PILLARS; i++)
    {
        pillars[i].bottom.x = x;
        pillars[i].bottom.y = y;
        pillars[i].bottom.width = TotalWidth;
        pillars[i].bottom.height = 1000;

        pillars[i].top.x = x;
        pillars[i].top.y = y - 1000 - PillarGap;
        pillars[i].top.width = TotalWidth;
        pillars[i].top.height = 1000;

        pillars[i].isScored = false;

        x += PillarSpace;
        y += 50;
    }

    gameState.score = 0;
}

void Game::UpdatePhysics(Flappy &flappy)
{
    // Apply gravity
    flappy.velocity -= .1;

    // Calculate y
    flappy.rect.y -= flappy.velocity;

    // Apply speed
    flappy.rect.x++;

    if (flappy.rotationVelocity > 0)
    {
        flappy.rotationVelocity -= 1.0f;
        flappy.rotation += flappy.rotationVelocity;
        if (flappy.rotation > 360.0)
            flappy.rotation -= 360.0;
    }
}

void Game::UpdatePillars(Pillar *pillars, Flappy &flappy)
{
    bool doShift = pillars[0].bottom.x < flappy.rect.x - 160;

    if (doShift)
    {
        for (int i = 0; i < TOTAL_PILLARS - 1; i++)
        {
            pillars[i] = pillars[i + 1];
            pillars[i].isScored = false;
        }

        int y = GetRandomValue(120, SCREEN_HEIGHT - 20);
        pillars[TOTAL_PILLARS - 1].bottom.x += PillarSpace;
        pillars[TOTAL_PILLARS - 1].top.x += PillarSpace;
        pillars[TOTAL_PILLARS - 1].bottom.y = y;
        pillars[TOTAL_PILLARS - 1].top.y = y - 1000 - PillarGap;
        pillars[TOTAL_PILLARS - 1].isScored = false;
    }
}

bool Game::HandleScore(Pillar *pillars, Flappy &flappy, GameState &gameState)
{
    if (pillars[0].bottom.x < flappy.rect.x && !pillars[0].isScored)
    {
        pillars[0].isScored = true;
        gameState.score++;

        return true;
    }

    return false;
}

void Game::UpdateBackground(Background &background)
{
    background.parallaxX -= 0.5f;
    if (background.parallaxX <= -128.0f)
        background.parallaxX += 128.0f;
}

void Game::UpdateBanner(Label &banner)
{
    banner.x -= 0.5f;
    if (banner.x <= -300.0f)
    {
        banner.x = SCREEN_WIDTH + 200;
        RandomizeBanner(banner);
    }
}

void Game::RandomizeBanner(Label &banner)
{
    banner.rotation = GetRandomValue(0, 180);
    banner.text = _bannerMessageSource.GetRandom();
}

void Game::UpdateDeathState(Flappy &flappy, Pillar *pillars)
{
    flappy.isDead = 
        CheckCollisionRecs(flappy.rect, pillars[0].bottom)
        || CheckCollisionRecs(flappy.rect, pillars[0].top)
        || flappy.rect.y > SCREEN_HEIGHT * 2;

}

void Game::ShiftScreen(Flappy &flappy, Camera2D &camera, Pillar *pillars)
{
    if (flappy.rect.x > SCREEN_WIDTH)
    {
        flappy.rect.x -= SCREEN_WIDTH;
        camera.target.x -= SCREEN_WIDTH;
        for (int i = 0; i < TOTAL_PILLARS; i++)
        {
            pillars[i].bottom.x -= SCREEN_WIDTH;
            pillars[i].top.x -= SCREEN_WIDTH;
        }
    }
}

void Game::Initialize()
{
    SetRandomSeed((unsigned int)time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Flappy Oik");
    InitAudioDevice();
    SetExitKey(KEY_Q);
    SetTargetFPS(60);
}

void Game::Run() {
    GameState gameState;
    Flappy flappy;
    Pillar* pillars = new Pillar[TOTAL_PILLARS];
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

    Label banner;
    banner.x = 100;
    banner.y = 100;
    banner.rotation = GetRandomValue(0, 180);
    banner.fontSize = 40;
    banner.color = SKYBLUE;
    banner.shadowColor = BLUE;
    banner.isVisible = true;
    this->RandomizeBanner(banner);

    Camera2D camera;
    camera.zoom = 1.0f;
    camera.rotation = 0.0f;
    camera.offset = (Vector2) { 100.0f, SCREEN_HEIGHT / 2.0f };

    this->Reset(flappy, pillars, camera, gameState);

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

        // Handle inputs
        if (IsKeyPressed(KEY_SPACE))
        {
            flappy.isJumping = true;
            gameState.isPaused = gameState.deathTimer > 0;
        }

        if (IsKeyPressed(KEY_ESCAPE))
        {
            gameState.isPaused = true;
        }

        // Loop part that should be paused
        if (!gameState.isPaused && gameState.deathTimer <= 0)
        {
            if (flappy.isDead)
            {
                flappy.isDead = false;
                this->Reset(flappy, pillars, camera, gameState);
            }

            this->UpdatePhysics(flappy);
            this->UpdatePillars(pillars, flappy);
            
            if (this->HandleScore(pillars, flappy, gameState))
                sfxPlayer.Play(SfxType::Point);

            if (flappy.isJumping)
            {
                flappy.velocity = JumpVelocity;
                flappy.rotationVelocity = 45.0f;
                sfxPlayer.Play(SfxType::Jump);
            }

            this->UpdateBackground(background);
            this->UpdateBanner(banner);

            this->UpdateDeathState(flappy, pillars);

            if (flappy.isDead)
            {
                gameState.isPaused = true;
                gameState.deathTimer = 10.0f;
                sfxPlayer.Play(SfxType::Hit);
            }
        }

        if (gameState.deathTimer > 0)
            gameState.deathTimer -= deltaTime;

        // Camera follows player
        camera.target = (Vector2) { roundf(flappy.rect.x), roundf(SCREEN_HEIGHT / 2) };

        // Fix for webassembly cam jigering
        this->ShiftScreen(flappy, camera, pillars);

        // Reset values
        flappy.isJumping = false;

        // Update labels
        scoreLabel.text = TextFormat("Score: %d", gameState.score);
        deadLabel.isVisible = gameState.deathTimer > 0;
        deadLabel.text = TextFormat("You are DEAD!!! (%d)", (int)gameState.deathTimer);
        pauseLabel.isVisible = gameState.isPaused && !flappy.isDead;
        gameOverLabel.isVisible = gameState.isPaused && flappy.isDead && gameState.deathTimer <= 0;

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Render background and background objects
        backgroundRenderer.Render(background);
        labelRenderer.Render(banner);

        BeginMode2D(camera);

        // Render world objects
        flappyRenderer.Render(flappy);
        pillarRenderer.Render(pillars);

        EndMode2D();

        // Render HUD
        labelRenderer.Render(scoreLabel);
        labelRenderer.Render(deadLabel);
        labelRenderer.Render(pauseLabel);
        labelRenderer.Render(gameOverLabel);

        EndDrawing();
    }

    labelRenderer.Uninitialize();
    backgroundRenderer.Unitialize();
    pillarRenderer.Uninitialize();
    flappyRenderer.Uninitialize();
    sfxPlayer.Unitialize();
}

void Game::Uninitialize()
{
    CloseAudioDevice();
    CloseWindow();
}
