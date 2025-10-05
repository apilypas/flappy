#include "FlappyRenderer.hpp"

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
            flappy.rect.x + 16,
            flappy.rect.y + 16,
            flappy.rect.width,
            flappy.rect.height
        },
        { 16, 16 },
        flappy.rotation,
        WHITE);
}

void FlappyRenderer::Uninitialize()
{
    UnloadTexture(_texture);
}
