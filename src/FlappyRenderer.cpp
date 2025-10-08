#include "FlappyRenderer.hpp"
#include <raylib.h>

void FlappyRenderer::Initialize()
{
    _texture = LoadTexture("assets/images/Oik.png");
}

void FlappyRenderer::Render(Flappy &flappy)
{
    DrawTexturePro(
        _texture,
        { 0, 0, 32, 32 },
        { 
            flappy.center.x,
            flappy.center.y,
            32.0f,
            32.0f
        },
        { 16, 16 },
        flappy.rotation,
        WHITE);
}

void FlappyRenderer::Uninitialize()
{
    UnloadTexture(_texture);
}
