#include "PowerUpRenderer.hpp"
#include "Entities.hpp"

void PowerUpRenderer::Initialize()
{
    _texture = LoadTexture("assets/images/PowerUps.png");
}

void PowerUpRenderer::Render(PowerUp &powerUp)
{
    if (powerUp.type == PowerUpType::Slow)
    {
        DrawTexturePro(
            _texture,
            { 16, 16, 32, 32 },
            powerUp.rect,
            { 0, 0 },
            0.0f,
            WHITE);
    }
    else if (powerUp.type == PowerUpType::Points)
    {
        DrawTexturePro(
            _texture,
            { 48, 16, 32, 32 },
            powerUp.rect,
            { 0, 0 },
            0.0f,
            WHITE);
    }
}

void PowerUpRenderer::Uninitialize()
{
    UnloadTexture(_texture);
}
