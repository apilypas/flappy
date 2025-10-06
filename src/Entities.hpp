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

struct Pillar
{
    Rectangle top = { 0, 0, 0, 0 };
    Rectangle bottom = { 0, 0, 0, 0 };
    bool isScored = false;
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
    int fontSize = 0;
    Color color = BLACK;
    Color shadowColor = BLACK;
    bool isVisible = false;
    float rotation = 0.0f;
};
