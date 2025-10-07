#pragma once
#include "Constants.hpp"
#include "raylib.h"

struct GameState
{
    bool isPaused = true;
    int score = 0;
    float deathTimer = 0.0f;
};

struct Flappy
{
    Rectangle rect = { 0, 0, 0, 0 };
    float velocity = 0.0f;
    float rotation = 0.0f;
    float rotationVelocity = 0.0f;
    bool isJumping = false;
    bool isDead = false;
    float speed = INITIAL_SPEED;
};

enum class PillarType
{
    Normal,
    Sliding
};

struct Pillar
{
    int id = 0;
    PillarType type = PillarType::Normal;
    Rectangle top = { 0, 0, 0, 0 };
    Rectangle bottom = { 0, 0, 0, 0 };
    bool isScored = false;
    bool hasPowerUp = false;
    bool isSlidingUp = false;
    float slidingSpeed = 60.0f;
};

struct Background
{
    float parallaxX = 0.0f;
};

struct Label
{
    const char *text = "";
    float x = 0;
    float y = 0;
    float fontSize = 0;
    Color color = BLACK;
    Color shadowColor = BLACK;
    bool isVisible = false;
    float rotation = 0.0f;
};

enum class PowerUpType
{
    Slow,
    Points
};

struct PowerUp
{
    PowerUpType type = PowerUpType::Slow;
    Rectangle rect = { 0.0f, 0.0f, 0.0f, 0.0f };
};
