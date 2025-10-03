#include "FlappyRenderer.hpp"

void FlappyRenderer::Initialize()
{
    _texture = LoadTexture("assets/images/Oik.png");
}

void FlappyRenderer::Render(Flappy &flappy)
{
    auto sourceRect = (Rectangle) { 0, 0, 32, 32 };
    auto destRect = (Rectangle) { 
        flappy.rect.x + 16,
        flappy.rect.y + 16,
        flappy.rect.width,
        flappy.rect.height
    };
    auto origin = (Vector2) { 16, 16 };

    DrawTexturePro(
        _texture,
        sourceRect,
        destRect,
        origin,
        flappy.rotation,
        WHITE);
}

void FlappyRenderer::Uninitialize()
{
    UnloadTexture(_texture);
}
