#include "BackgroundRenderer.hpp"

void BackgroundRenderer::Initialize()
{
    _texture = LoadTexture("assets/images/Sky.png");
}

void BackgroundRenderer::Render(Background &background)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            DrawTexture(
                _texture,
                background.parallaxX + i * 128,
                j * 128,
                WHITE);
        }
    }
}

void BackgroundRenderer::Unitialize()
{
    UnloadTexture(_texture);
}
